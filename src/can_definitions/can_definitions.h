/*
 * can_definitions.h
 *
 * Created: 16.10.2023 14:15:30
 *  Author: jknewall
 */ 


#ifndef CAN_DEFINITIONS_H_
#define CAN_DEFINITIONS_H_


typedef enum {
	CAN_ID_CALIBRATE_ENCODER = 0x97,
	CAN_ID_MOTOR_TO_START = 0x98,
	CAN_ID_GOAL_SCORED = 0x99,
	CAN_ID_JOYSTICK_STATE = 0x100,
	CAN_ID_MOTOR_DISABLE = 0x101,
} can_id_t;

typedef struct {
	int16_t stick_horizontal_pos;
	int16_t stick_vertical_pos;
	uint16_t slider_pos;
	uint8_t button_state; // 1 = pressed, 0 = depressed
} can_data_gamepad_state_t;

typedef struct {
} can_data_is_goal_t;

#endif /* CAN_DEFINITIONS_H_ */