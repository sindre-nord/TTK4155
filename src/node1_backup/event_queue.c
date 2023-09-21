/*
 * event.c
 *
 * Created: 19.09.2023 13:45:43
 *  Author: brageim
 */ 
#include "event_queue.h"

#include <avr/interrupt.h>

#include "circular_buffer.h"

static volatile circular_buffer_t event_queue;

uint8_t eq_initialize(void) {
	return cb_allocate(&event_queue, 8, sizeof(event_t));
}

void eq_push_event(event_t pushed_event) {
	cli();
	cb_push_back(&event_queue, &pushed_event);
	sei();
}

cb_pop_result_t eq_pop_next_event(event_t* next) {
	cli();
	cb_pop_result_t out = cb_pop_front(&event_queue, &next);
	sei();
	return out;
}
