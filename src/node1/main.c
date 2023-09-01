/*
 * node1.c
 *
 * Created: 01.09.2023 14:43:35
 * Author : Group 21
 */ 

#include <avr/io.h>
#include <stdbool.h>

#include "uart_driver.h"

#define CLOCK_SPEED 4915200 // Hz
#define BAUD_RATE 9600


int main(void)
{
    uart_init(CLOCK_SPEED, BAUD_RATE);
	while (true) {
		uart_transmit(uart_receive());
	}
}


