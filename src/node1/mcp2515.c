/*
 * mcp2515.c
 *
 * Created: 28.09.2023 12:38:19
 *  Author: jknewall
 */ 

#include "mcp2515.h"

#include <stdbool.h>

#include "spi.h"

#include <stdio.h>

#define NUM_RX_BUFFERS 2
#define NUM_TX_BUFFERS 3
// Using 0 for now. Other buffers can be selected dynamically based on which are available based on TXnRTS pins n=0,1,2 later if wanted
#define USED_TX_BUFFER 0
#define MCP_INTERRUPT_PORT PORTB
#define MCP_INTERRUPT_PIN PINB3

#define MCP_CAN_BAUD_RATE 19200ULL

#define MCP_F_OSC 16000000ULL
#define MCP_SJW 0 // Sync = (SJW + 1) x T_Q  (Sync = Synchronization Jump Width Length)
#define MCP_PRSEG 2 // PropSeg = (PRSEG + 1) x TQ
#define MCP_PHSEG1 2 // PS1 = (PHSEG1 + 1) x TQ
#define MCP_PHSEG2 2 // PS2 = (PHSEG2 + 1) x TQ
#define MCP_BRP (uint8_t)(MCP_F_OSC/(2*MCP_CAN_BAUD_RATE*(unsigned long long)(MCP_SJW+MCP_PRSEG+MCP_PHSEG1+MCP_PHSEG2+4)) - 1) // T_Q = 2 x (BRP + 1)/F_OSC ^ 1/BAUD_RATE = (Sync+PropSeg+PS1+PS2) => BRP = FOSC/(2*BAUD_RATE*(SJW+PRSEG+PHSEG1+PHSEG2+4)) - 1

void mcp_init(void) {
	spi_master_init();
	printf("BRP: %u\n", MCP_BRP);
	mcp_write_addressed(MCP_REG_CAN_CONTROL, 0b10000000); // Set configuration mode
	
	//unsigned char config[] = {(MCP_SJW<<6)|MCP_BRP, (1<<7)|MCP_PHSEG1, MCP_PHSEG2};
	mcp_write_addressed(MCP_REG_CONFIGURATION_1, (MCP_SJW<<6)|MCP_BRP);
	mcp_write_addressed(MCP_REG_CONFIGURATION_2, (1<<7)|(MCP_PHSEG1<<3)|MCP_PRSEG);
	mcp_write_addressed(MCP_REG_CONFIGURATION_3, MCP_PHSEG2);

	mcp_write_addressed(MCP_REG_CAN_CONTROL, 0b00000000); // Set normal operation mode
	
	mcp_bit_modify(MCP_REG_INTERRUPT_ENABLE, 0b11111111, 1<<(USED_TX_BUFFER+NUM_RX_BUFFERS) & 0b11);
}

unsigned char mcp_read_addressed(uint8_t address) {
	spi_begin_transaction();

	spi_master_transmit(MCP_CMD_READ);
	spi_master_transmit(address);
	spi_master_transmit('r');
	
	spi_end_transaction();
	return spi_read_buffer();	
}

void mcp_read(unsigned char data[], uint8_t length) {
	for (uint8_t i = 0; i < length; i++) {
		spi_master_transmit('r');
		data[i] = spi_read_buffer();
	}
}

void mcp_write(unsigned char data[], uint8_t length) {
	for (uint8_t i = 0; i < length; i++) {
		spi_master_transmit(data[i]);
	}
}

void mcp_write_addressed(uint8_t address, unsigned char data) {
	spi_begin_transaction();

	spi_master_transmit(MCP_CMD_WRITE);
	spi_master_transmit(address);
	spi_master_transmit(data);

	spi_end_transaction();
}

void mcp_bit_modify(uint8_t address, unsigned char mask, unsigned char data) {
	spi_begin_transaction();

	spi_master_transmit(MCP_CMD_BIT_MODIFY);
	spi_master_transmit(address);
	spi_master_transmit(mask);
	spi_master_transmit(data);

	spi_end_transaction();
}

int8_t mcp_check_for_full_rx_buffer(void) {
	if (!(MCP_INTERRUPT_PORT & (1<<MCP_INTERRUPT_PIN))) {
		unsigned char rx_interrupt_flags = mcp_read_addressed(MCP_REG_INTERRUPT_FLAG) & 0b11;
		if (rx_interrupt_flags & 0b1) {
			return 0;
		}
		if (rx_interrupt_flags >> 1) {
			return 1;
		}
	}
	return -1;
}

uint8_t mcp_wait_for_full_rx_buffers(void) {
	while (true) {
		int8_t full_buffer = mcp_check_for_full_rx_buffer();
		if (full_buffer != -1) {
			return (uint8_t) full_buffer;
		}
	}
}

void mcp_read_standard_id_field(int8_t rx_buffer_index, uint16_t* id) {
	spi_begin_transaction();

	spi_master_transmit(MCP_CMD_READ_RX_BUFFER | (rx_buffer_index<<2));
	spi_master_transmit('r');
	unsigned char id_msb = spi_read_buffer();
	spi_master_transmit('r');
	unsigned char id_lsb = spi_read_buffer();
	*id = (id_msb<<3) | (id_lsb>>5);

	spi_end_transaction();
}

uint8_t mcp_read_data_field(int8_t rx_buffer_index, unsigned char data[]) {
	spi_begin_transaction();

	spi_master_transmit(MCP_CMD_READ);
	spi_master_transmit(MCP_REG_RX_DLC_BASE + 0x10*rx_buffer_index);
	spi_master_transmit('r');
	uint8_t data_length = spi_read_buffer() & 0b1111;
	// The directly following addresses are for data
	mcp_read(data, data_length);
	
	spi_end_transaction();
	return data_length;
}

void mcp_signal_available_rx_buffer(uint8_t rx_buffer_index) {
	mcp_bit_modify(MCP_REG_INTERRUPT_FLAG, 1<<rx_buffer_index, 0);
}

uint8_t mcp_wait_for_empty_tx_buffers(void) {
	static bool is_first_tx = true;
	uint8_t available_tx_buffer_index = USED_TX_BUFFER; // Only use this for now, for simplicity
	while (!is_first_tx && !(MCP_INTERRUPT_PORT & (1<<MCP_INTERRUPT_PIN))) {
		unsigned char interrupt_flags = mcp_read_addressed(MCP_REG_INTERRUPT_FLAG);
		unsigned char tx_interrupt_flag_mask = 1<<(available_tx_buffer_index+NUM_RX_BUFFERS);
		if (interrupt_flags & tx_interrupt_flag_mask) {
			mcp_bit_modify(MCP_REG_INTERRUPT_FLAG, tx_interrupt_flag_mask, 0);
			break;
		}
	}
	is_first_tx = false;
	return available_tx_buffer_index;
}

void mcp_write_standard_id_field(uint8_t tx_buffer_index, uint16_t id) {
	spi_begin_transaction();

	spi_master_transmit(MCP_CMD_LOAD_TX_BUFFER | (tx_buffer_index<<1));
	spi_master_transmit(id>>3);
	spi_master_transmit(id<<5);

	spi_end_transaction();
}

void mcp_write_data_field(uint8_t tx_buffer_index, unsigned char data[], uint8_t length) {
	spi_begin_transaction();

	spi_master_transmit(MCP_CMD_WRITE);
	spi_master_transmit(MCP_REG_TX_DLC_BASE + 0x10*tx_buffer_index);
	spi_master_transmit((unsigned char)length);
	// The directly following addresses are for data
	mcp_write(data, length);

	spi_end_transaction();
}

void mcp_request_to_send(uint8_t tx_buffer_index) {
	spi_begin_transaction();
	spi_master_transmit(MCP_CMD_REQUEST_TO_SEND | 1<<tx_buffer_index);
	spi_end_transaction();
}


//void mcp_load_tx_buffer(uint8_t selected_buffer)