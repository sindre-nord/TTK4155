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
#include "oled_driver.h"
#include "player_input_driver.h"
#include "state_machine.h"
#include "states.h"
#include "uart_driver.h"
#include "hardware_tests.h"

#define BAUD_RATE 9600

#include "fonts.h"

void main_render(void) {
	//oled_clear();
	//FILE* prior_stdout = stdout;
	stdout = fdevopen(oled_putchar, NULL);
	printf("Raaa tekst");
// 	for (uint8_t entry = 0; entry < menu_p->num_entries; entry++) {
// 		oled_select_segment(entry, 0);
// 		if (entry == menu_p->selected_entry) {
// 			menu_render_cursor();
// 			} else {
// 			menu_clear_cursor();
// 		}
// 		for (uint8_t i = 0; i<8;i++){
// 			oled_putchar(menu_p->entries[entry].display_text[i], NULL);
// 		}
// 		//printf(menu_p->entries[entry].display_text);
// 		//printf("Raa tekst");
// 	}
	//stdout = prior_stdout;
}

int main(void)
{
	MCUCR |= 1 << SRE; // Enable external memory
	SFIOR |= 1 << XMM2; // Stop using PC7-PC4 for memory as they have JTAG connected (see Atmega datasheet table 4)

	uart_init(F_CPU, BAUD_RATE);
	sei();
	adc_init();
	oled_init();

	oled_clear();
	
	stdout = fdevopen(oled_putchar, NULL);
	oled_select_segment(5,10);
	printf("Loading");
	_delay_ms(500);
	printf(".");
	_delay_ms(500);
	printf(".");
	_delay_ms(500);
	printf(".");
	_delay_ms(1000);
	
	
	
	
	stdout = fdevopen(uart_putchar, NULL);
	printf("Starting up...\n");

	fsm_t fsm_instance;
    fsm_t* fsm_pointer = &fsm_instance;
    
    fsm_initialize(fsm_pointer, state_menu);
    fsm_dispatch(fsm_pointer, EVENT_ENTRY);
	_delay_ms(500);
	
	

	unsigned char received[100];
	size_t num_received = 0;
	
	//SRAM_test();
	
	joystick_direction_t joystick_direction;
	while (true) {
		
		joystick_direction = get_joystick_direction();
		
		
		
		printf("Joystick: %i\n", joystick_direction);
		
		if(joystick_direction == UP){
			printf("Up");
			fsm_dispatch(fsm_pointer, EVENT_JOY_UP);
		}else if(joystick_direction == DOWN){
			printf("Down");
			fsm_dispatch(fsm_pointer, EVENT_JOY_DOWN);
		} else if(joystick_direction == RIGHT){
			printf("Right");
			fsm_dispatch(fsm_pointer, EVENT_JOY_RIGHT);
		} else if(joystick_direction == LEFT){
			printf("Left");
			fsm_dispatch(fsm_pointer, EVENT_JOY_LEFT);
		}
		// } else if(joystick_direction == CLICK){
		// 	printf("Click");
		// 	fsm_dispatch(fsm_pointer, EVENT_JOY_CLICK);
		// }
		
		_delay_ms(500);
		
		
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

