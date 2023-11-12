/*
 * mcp2515.h
 *
 * Created: 28.09.2023 12:38:08
 *  Author: jknewall
 */ 

#ifndef MCP2515_H_
#define MCP2515_H_

#include <stdint.h>

typedef enum {
	MCP_CMD_RESET = 0b11000000,
	MCP_CMD_READ = 0b00000011,
	MCP_CMD_WRITE = 0b00000010,
	MCP_CMD_BIT_MODIFY = 0b00000101,
	MCP_CMD_READ_RX_BUFFER = 0b10010000, // Incomplete, must specify the 2 least significant bits
	MCP_CMD_LOAD_TX_BUFFER = 0b01000000, // Incomplete, must specify the 3 least significant bits
	MCP_CMD_REQUEST_TO_SEND = 0b10000000,
	MCP_CMD_READ_STATUS = 0b10110000,
	
} mcp_command_t;

typedef enum {
	MCP_REG_CAN_CONTROL = 0x0F,
	MCP_REG_CONFIGURATION_1 = 0x2A,
	MCP_REG_CONFIGURATION_2 = 0x29,
	MCP_REG_CONFIGURATION_3 = 0x28,
	MCP_REG_INTERRUPT_ENABLE = 0x2B,
	MCP_REG_INTERRUPT_FLAG = 0x2C,
	MCP_REG_TX_DLC_BASE = 0x35, // Next registers spaced by n*0x10
	MCP_REG_TX_DATA_BASE = 0x36, // Next registers spaced by n*0x10
	MCP_REG_RX_DLC_BASE = 0x65, // Next registers spaced by n*0x10
} mcp_register_t;

void mcp_init(void);

void mcp_read(unsigned char data[], uint8_t length);
unsigned char mcp_read_addressed(unsigned char address);
void mcp_write(unsigned char data[], uint8_t length);
void mcp_write_addressed(uint8_t address, unsigned char data);

// The 1s in the mast will specify which bits in the data byte are setting the register given by address
void mcp_bit_modify(uint8_t address, unsigned char mask, unsigned char data);


// Returns index of a full rx buffer or -1 if there is no full buffer
int8_t mcp_check_for_full_rx_buffer(void);
uint8_t mcp_wait_for_full_rx_buffers(void);

void mcp_read_standard_id_field(int8_t rx_buffer_index, uint16_t* id);
uint8_t mcp_read_data_field(int8_t rx_buffer_index, unsigned char data[]);
void mcp_signal_available_rx_buffer(uint8_t rx_buffer_index);

// Returns index of an available tx buffer
uint8_t mcp_wait_for_empty_tx_buffers(void);
void mcp_write_standard_id_field(uint8_t tx_buffer_index, uint16_t id);
void mcp_write_data_field(uint8_t tx_buffer_index, unsigned char data[], uint8_t length);
void mcp_request_to_send(uint8_t tx_buffer_index);


#endif /* MCP2515_H_ */