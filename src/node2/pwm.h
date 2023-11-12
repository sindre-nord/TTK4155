/*
 * pwm.h
 *
 * Created: 16.10.2023 15:50:48
 *  Author: jknewall
 */ 


#ifndef PWM_H_
#define PWM_H_

#include <stdint.h>

//Enable PWM clock in PMC

void pwm_init(void);

void pwm_set_angle(uint16_t angle);


#endif /* PWM_H_ */