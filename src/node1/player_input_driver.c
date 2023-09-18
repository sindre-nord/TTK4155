#include "player_input_driver.h"

// Include standard libs
#include <avr/io.h>

// Include custom libs
#include "adc_driver.h"

static uint8_t calibration_offset = 162;

int16_t read_joy_stick_axis(joystick_axis_t axis) {
    return adc_read(axis)-calibration_offset;
}

uint8_t read_slider(slider_side_t side) {
	return adc_read(side);
}