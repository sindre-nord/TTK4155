/*
 * node1.c
 *
 * Created: 01.09.2023 14:43:35
 * Author : Group 21
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#define F_CPU 4915200 // Hz
#include <util/delay.h> // Uses F_CPU
#include "adc_driver.h"
#include "uart_driver.h"
#include "hardware_tests.h"

#define BAUD_RATE 9600


int main(void)
{
	MCUCR |= 1 << SRE; // Enable external memory
	SFIOR |= 1 << XMM2; // Stop using PC7-PC4 for memory as they have JTAG connected (see Atmega datasheet table 4)
	
	stdout = fdevopen(uart_putchar, NULL);
	uart_init(F_CPU, BAUD_RATE);
	sei();
	adc_init();

	printf("Starting up...\n");
	
	unsigned char received[100];
	size_t num_received = 0;
	while (true) {
		num_received = uart_receive(received, 100);
		if(num_received > 0) {
			printf("Reading from ADC:\nChannel 0: %u\nChannel 1: %u\nChannel 2: %u\nChannel 3: %u\n\n", adc_read(0), adc_read(1), adc_read(2), adc_read(3));
		}
		uart_transmit(received, num_received);
		_delay_ms(100);
	}
}

