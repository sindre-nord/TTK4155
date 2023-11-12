/*
 * timer.c
 *
 * Created: 16.10.2023 13:15:49
 *  Author: jknewall
 */

#include "timer.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include "event_queue.h"

#define TIMER_COUNTER_RESET_VALUE (65536 - 48)  // for 0.01 sec at F_CPU

void timer_init(void) {
	TCNT1 = TIMER_COUNTER_RESET_VALUE;

	TCCR1A = 0x00;
	TCCR1B = (1<<CS10) | (1<<CS12);;  // Timer mode with 1024 prescaler
	TIMSK = (1 << TOIE1) ;   // Enable timer1 overflow interrupt(TOIE1)
}

ISR (TIMER1_OVF_vect)    // Timer1 ISR
{
	TCNT1 = TIMER_COUNTER_RESET_VALUE;
	static uint8_t counter = 0;
	if (counter == 100) {
		counter = 0;
		eq_push_event(EVENT_TIMER_1HZ);
	}
	eq_push_event(EVENT_TIMER_100HZ);
	if (counter % 2 == 0) {
		eq_push_event(EVENT_TIMER_50HZ);
	}
	if (counter % 5 == 0) {
		eq_push_event(EVENT_TIMER_20HZ);
	}
	if (counter % 10 == 0) {
		eq_push_event(EVENT_TIMER_10HZ);
	}
	if (counter % 20 == 0) {
		eq_push_event(EVENT_TIMER_5HZ);
	}
	counter++;
}
