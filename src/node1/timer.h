/*
 * timer.h
 *
 * Created: 16.10.2023 13:16:17
 *  Author: jknewall
 */ 


#ifndef TIMER_H_
#define TIMER_H_

#define F_CPU 4915200 // Hz
#include <util/delay.h> // Uses F_CPU


void timer_init(void);

#endif /* TIMER_H_ */