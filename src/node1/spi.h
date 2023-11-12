/*
 * SPI.h
 *
 * Created: 28.09.2023 11:09:01
 *  Author: jknewall
 */ 


#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>
#include <avr/io.h>

typedef enum{
	SPI_PIN_SS = DDB4,
	SPI_PIN_MOSI = DDB5,
	SPI_PIN_MISO = DDB6,
	SPI_PIN_SCK = DDB7,  
}spi_pin_t;


void spi_master_init(void);

void spi_begin_transaction(void);

void spi_end_transaction(void);

uint8_t spi_read_buffer(void);

void spi_master_transmit(unsigned char data);


#endif /* SPI_H_ */