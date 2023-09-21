#pragma once

// This driver needs to give access to values from the different player inputs.

#include <stdint.h>

typedef enum {
	ADC_CHANNEL0 = 0,
	ADC_CHANNEL1 = 1,
	ADC_CHANNEL2 = 2,
	ADC_CHANNEL3 = 3
} adc_channel_t;

// From the adc:
// Returns a value between -128 to 127
typedef enum {
	JOY_HORIZONTAL = ADC_CHANNEL0, // Negative = left, positive = right
	JOY_VERTICAL = ADC_CHANNEL1  // Negative = down, positive = up
} joystick_axis_t;
int16_t read_joy_stick_axis(joystick_axis_t axis);

typedef enum {
	SLIDER_RIGHT = ADC_CHANNEL2,
	SLIDER_LEFT = ADC_CHANNEL3
} slider_side_t;
uint8_t read_slider(slider_side_t side);

// From IO
int left_button(void);
int right_button(void);
