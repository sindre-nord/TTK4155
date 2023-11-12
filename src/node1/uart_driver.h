/**
 * @file uart_driver.h
 * @author Sindre Nordtveit (sanordtv@ntnu.no)
 * @brief Hold a simple UART driver for the ATmega162 for use in TTK4155
 * @version 0.1
 * @date 2023-09-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */


#pragma once
#include <avr/io.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

/**
 * @brief Initialize UART
 * 
 * @param clock_speed Clock speed of the MCU
 * @param baud_rate Baud rate of the UART
 */
void uart_init(uint32_t clock_speed, uint32_t baud_rate);
void uart_transmit(unsigned char bytes[], size_t num_bytes);
size_t uart_receive(unsigned char bytes_out[], size_t max_bytes);

extern FILE* uart_stdout;
int uart_putchar(char c, FILE* stream);

