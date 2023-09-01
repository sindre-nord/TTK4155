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

#include "uart_driver.h"

#define BAUD_RATE 9600


int main(void)
{
    uart_init(F_CPU, BAUD_RATE);
	stdout = fdevopen(uart_putchar, NULL);

	sei();
	
	printf("Starting up...\n");
	unsigned char received[100];
	size_t num_received = 0;
	while (true) {
		num_received = uart_receive(received, 100);
		uart_transmit(received, num_received);
		_delay_ms(100);
	}
}

