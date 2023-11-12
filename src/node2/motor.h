/*
 * motor.h
 *
 * Created: 23.10.2023 14:28:47
 *  Author: jknewall
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_

#include <stdint.h>

void delay_20us(void);
void motor_init(void);
void motor_set_speed(int16_t speed);
void motor_controller_reset(void);
void motor_control_position(uint16_t pos);
void motor_enable(void);
void motor_disable(void);
void encoder_calibrate(void);
uint16_t motor_read_position(void);

void dac_init(void);
void dac_set_value(uint16_t value);

void solenoid_init(void);
void solenoid_enable(void);
void solenoid_disable(void);

#endif /* MOTOR_H_ */