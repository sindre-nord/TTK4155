#include "uart_driver.h"

#include <avr/interrupt.h>
#include <stdbool.h>

#include "circular_buffer.h"

static circular_buffer_t tx_buffer;
static circular_buffer_t rx_buffer;

void uart_init(uint32_t clock_speed, uint32_t baud_rate)
{
    uint16_t ubrr = ((clock_speed/16)/baud_rate)-1;
	// Set baud rate
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	// Enable transmission and reception
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	// Frame format: 8data, 2stop bit
	UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);
	
	cb_allocate(&tx_buffer, 100, sizeof(unsigned char));
	cb_allocate(&rx_buffer, 100, sizeof(unsigned char));
	
	// Enable transmission and reception interrupts. This must be done after everything
	// else is initialized to use the correct settings as this will start TX/RX immediately
	UCSR0B |= (1<<RXCIE0)|(1<<TXCIE0);
}

ISR(USART0_TXC_vect)
{
	unsigned char byte;
	if (cb_pop_front(&tx_buffer, &byte) == CB_POP_SUCCESSFUL) {
		UDR0 = byte;
	}
}

void uart_transmit(unsigned char bytes[], size_t num_bytes)
{
	cli(); // Disable interrupts as a synchronization mechanism
	for (int i = 0; i < num_bytes; i++) {
		cb_push_back(&tx_buffer, &bytes[i]);
	}
	sei(); // Re-enable interrupts
	unsigned char byte;
	if ((UCSR0A & (1<<UDRE0)) && cb_pop_front(&tx_buffer, &byte) == CB_POP_SUCCESSFUL) {
		// Transmit buffer is empty
		UDR0 = byte;
	}
}

ISR(USART0_RXC_vect)
{
	unsigned char byte = UDR0;
	cb_push_back(&rx_buffer, &byte);
}

size_t uart_receive(unsigned char bytes[], size_t max_bytes)
{
	size_t bytes_read = 0;
	cli(); // Disable interrupts as a synchronization mechanism
	while (rx_buffer.count > 0 && bytes_read <= max_bytes) {
		cb_pop_front(&rx_buffer, &bytes[bytes_read]);
		bytes_read++;
	} // Re-enable interrupts
	sei();
	return bytes_read;
}