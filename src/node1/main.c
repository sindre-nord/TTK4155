/*
 * node1.c
 *
 * Created: 01.09.2023 14:43:35
 * Author : Group 21
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#include "../can_definitions/can_definitions.h"
#include "adc_driver.h"
#include "can.h"
#include "event_queue.h"
#include "oled_driver.h"
#include "player_input_driver.h"
#include "state_machine.h"
#include "states.h"
#include "timer.h"
#include "uart_driver.h"
#include "hardware_tests.h"
#include "mcp2515.h"

#define BAUD_RATE 9600



int main(void)
{
	MCUCR |= 1 << SRE; // Enable external memory
	SFIOR |= 1 << XMM2; // Stop using PC7-PC4 for memory as they have JTAG connected (see Atmega datasheet table 4)
	
	// Debug trigger pin
	//DDRB |= 1<<2;
	//PORTB |= 1<<2;
	//PORTB &= ~1<<2;
	
	timer_init();
	uart_init(F_CPU, BAUD_RATE);
	can_init();
	sei();
	adc_init();
	oled_init();

	stdout = uart_stdout;
	printf("Starting up...\n");

	oled_display_loading_screen();

	stdout = uart_stdout;
	unsigned char control_reg = mcp_read_addressed(MCP_REG_CAN_CONTROL);
	printf("Can control register value: %u\n\n", control_reg);
	

/*
	can_frame_t sent_frame = {.id=0b10101010101, .data_length=8};
	for (uint8_t i = 0; i < sent_frame.data_length; i++) {
		sent_frame.data[i] = i;
	}
	can_send(&sent_frame);
	*/
	
	



	fsm_t fsm;

	eq_initialize();
    fsm_initialize(&fsm, state_menu);

	event_t next_event;
	//unsigned char received_uart[100];
	//size_t num_received_uart = 0;
	can_frame_t received_frames[1];
	size_t num_received_frames = 0;
	
	
	while (true) {
		player_input_read_all();
		num_received_frames = can_receive(received_frames, 1);
		if (num_received_frames != 0) {
			for (uint8_t i = 0; i < num_received_frames; i++) {
				can_frame_t* received_frame = &received_frames[i];
				switch (received_frame->id) {
					case CAN_ID_GOAL_SCORED:
						eq_push_event(EVENT_GOAL_SCORED);
						break;
					default:
						printf("Received frame of ID %u and data length %u:\n", received_frame->id, received_frame->data_length);
						for (uint8_t i = 0; i < received_frame->data_length; i++) {
							printf("%u, ", received_frame->data[i]);
						}
						printf("\n");
						break;
				}
				
			}
		}
		if (eq_pop_next_event(&next_event) == CB_POP_SUCCESSFUL) {
			fsm_dispatch(&fsm, next_event);
		}
		
// 		num_received = uart_receive(received, 100);
// 		if(num_received > 0) {
// 			printf("Reading from ADC:\nChannel 0: %u\nChannel 1: %u\nChannel 2: %u\nChannel 3: %u\n\n", adc_read(0), adc_read(1), adc_read(2), adc_read(3));
// 			printf("Joystick x: %i\n", read_joy_stick_axis(JOY_HORIZONTAL));
// 			printf("Right slider: %u\n", read_slider(SLIDER_RIGHT));
// 		}
// 		uart_transmit(received, num_received);
// 		_delay_ms(100);
	}
}

