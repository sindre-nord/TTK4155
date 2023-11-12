/*
 * SPI.c
 *
 * Created: 28.09.2023 11:08:53
 *  Author: jknewall
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/portpins.h>
#include "spi.h"
	
void spi_master_init(void){

	DDRB |= (1<<SPI_PIN_MOSI) | (1<<SPI_PIN_SCK) | (1<<SPI_PIN_SS);
	
	SPCR = (1<<SPE) | (1<<MSTR);
	PORTB |= (1<<SPI_PIN_SS);
}

uint8_t spi_read_buffer(void){
	return SPDR;
}

void spi_begin_transaction(void){
	PORTB &= ~(1<<SPI_PIN_SS);
}

void spi_end_transaction(void){
	PORTB |= (1<<SPI_PIN_SS);
}

// Assumes SS active
void spi_master_transmit(unsigned char data){
	SPDR = data;
	while (!(SPSR & (1<<SPIF))){
		;
	}
}