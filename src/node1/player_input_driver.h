#pragma once

// This driver needs to give access to values from the different player inputs.

#include <stdint.h>

typedef enum {
	ADC_CHANNEL0 = 0,
	ADC_CHANNEL1 = 1,
	ADC_CHANNEL2 = 2,
	ADC_CHANNEL3 = 3
} adc_channel_t;

typedef enum {
	JOY_UP,
	JOY_DOWN,
	JOY_LEFT,
	JOY_RIGHT,
	JOY_UP_LEFT,
	JOY_UP_RIGHT,
	JOY_DOWN_LEFT,
	JOY_DOWN_RIGHT,
	JOY_NEUTRAL
} joystick_direction_t;
typedef enum {
	JOY_HORIZONTAL, // Negative = left, positive = right
	JOY_VERTICAL,  // Negative = down, positive = up
	JOY_NUM_AXES,
} joystick_axis_t;
typedef enum {
	SLIDER_RIGHT,
	SLIDER_LEFT,
	SLIDER_COUNT
} slider_side_t;

typedef struct {
	int16_t joy_stick[JOY_NUM_AXES];
	uint8_t slider[SLIDER_COUNT];
} player_input_t;

extern volatile player_input_t player_input;

// From the adc:
// Returns a value between -128 to 127
int16_t read_joy_stick_axis(joystick_axis_t axis);
uint8_t read_slider(slider_side_t side);
void player_input_read_all(void);

// From IO. Returns 1 if pressed, 0 if depressed
uint8_t read_left_button(void);
uint8_t read_right_button(void);

joystick_direction_t get_joystick_direction(void);
