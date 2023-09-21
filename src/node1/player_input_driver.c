#include "player_input_driver.h"

// Include standard libs
#include <avr/io.h>

// Include custom libs
#include "adc_driver.h"

//static uint8_t calibration_offset = 162;
static uint8_t calibration_offset = 0;

int16_t read_joy_stick_axis(joystick_axis_t axis) {
    return adc_read(axis)-calibration_offset;
}

uint8_t read_slider(slider_side_t side) {
	return adc_read(side);
}

joystick_direction_t get_joystick_direction(void){
	//Bad data types?
	
	const uint8_t left_border = 85;
	const uint8_t right_border = 170;
	const uint8_t up_border = 170;
	const uint8_t down_border = 85;
	
	int16_t horisontal_value = read_joy_stick_axis(JOY_HORIZONTAL);
	int16_t vertical_value = read_joy_stick_axis(JOY_VERTICAL);
	
	if(horisontal_value < left_border){
		if(vertical_value < down_border){
			return DOWN_LEFT;
		} else if(vertical_value > up_border){
			return UP_LEFT;
		} else{
			return LEFT;
		}
	} else if(horisontal_value > right_border){
		if(vertical_value < down_border){
			return DOWN_RIGHT;
		} else if(vertical_value > up_border){
			return UP_RIGHT;
		} else{
			return RIGHT;
		}
	}	else{
		if(vertical_value < down_border){
			return DOWN;
		} else if(vertical_value > up_border){
			return UP;
		} else{
			return NEUTRAL;
		}
	}
	
	
	
}

