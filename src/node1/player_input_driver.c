#include "player_input_driver.h"

// Include standard libs
#include <avr/io.h>
#include <stdbool.h>
#include <stdlib.h>

// Include custom libs
#include "adc_driver.h"
#include "event_queue.h"

static uint8_t calibration_offset = 161;

int16_t read_joy_stick_axis(joystick_axis_t axis) {
    return adc_read(axis==JOY_HORIZONTAL?ADC_CHANNEL0:ADC_CHANNEL1)-calibration_offset;
}

uint8_t read_slider(slider_side_t side) {
	return adc_read(side==SLIDER_LEFT?ADC_CHANNEL2:ADC_CHANNEL3);
}

volatile player_input_t player_input = {0};

void player_input_read_all(void) {
	static bool allow_stick_direction_event[JOY_NUM_AXES];
	uint8_t stick_direction_activation_threshold = 30;
	uint8_t stick_direction_deactivation_threshold = 20;
	
	for (joystick_axis_t a = 0; a < JOY_NUM_AXES; a++) {
		player_input.joy_stick[a] = read_joy_stick_axis(a);
		if (abs(player_input.joy_stick[a]) < stick_direction_deactivation_threshold) {
			allow_stick_direction_event[a] = true;
		} else if (allow_stick_direction_event[a]
		           && abs(player_input.joy_stick[a]) > stick_direction_activation_threshold) {
			if (player_input.joy_stick[a] > 0) {
				eq_push_event(a == JOY_HORIZONTAL ? EVENT_JOY_RIGHT : EVENT_JOY_UP);
			} else {
				eq_push_event(a == JOY_HORIZONTAL ? EVENT_JOY_LEFT : EVENT_JOY_DOWN);
			}
			allow_stick_direction_event[a] = false;
		}
	}
	for (slider_side_t s = 0; s < SLIDER_COUNT; s++) {
		player_input.slider[s] = read_slider(s);
	}
}

joystick_direction_t get_joystick_direction(void){
	
	const int16_t left_border = -30;
	const int16_t right_border = 30;
	const int16_t up_border = 30;
	const int16_t down_border = -30;
	
	int16_t horisontal_value = read_joy_stick_axis(JOY_HORIZONTAL);
	int16_t vertical_value = read_joy_stick_axis(JOY_VERTICAL);
	
	if(horisontal_value < left_border){
		if(vertical_value < down_border){
			return JOY_DOWN_LEFT;
		} else if(vertical_value > up_border){
			return JOY_UP_LEFT;
		} else{
			return JOY_LEFT;
		}
	} else if(horisontal_value > right_border){
		if(vertical_value < down_border){
			return JOY_DOWN_RIGHT;
		} else if(vertical_value > up_border){
			return JOY_UP_RIGHT;
		} else{
			return JOY_RIGHT;
		}
	}else{
		if(vertical_value < down_border){
			return JOY_DOWN;
		} else if(vertical_value > up_border){
			return JOY_UP;
		} else{
			return JOY_NEUTRAL;
		}
	}
}

uint8_t read_right_button(void) {
	return (PINB & (1<<PINB1)) >> PINB1;
}

