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
#include <stdint.h>

/**
 * @brief Initialize UART
 * 
 * @param clock_speed Clock speed of the MCU
 * @param baud_rate Baud rate of the UART
 */
void uart_init(uint32_t clock_speed, uint32_t baud_rate);
void uart_transmit(unsigned char data);
unsigned char uart_receive(void);

