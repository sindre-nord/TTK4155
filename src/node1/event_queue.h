/*
 * event.h
 *
 * Created: 19.09.2023 13:45:33
 *  Author: brageim
 */ 


#ifndef EVENT_QUEUE_H_
#define EVENT_QUEUE_H_

#include <stdint.h>

#include "circular_buffer.h"

typedef enum {
	//Events
	EVENT_NULL = 0, // Should not be queued, only there to make events optional in some cases. Must be 0.
	
	EVENT_ENTRY,
	EVENT_EXIT,
	
	EVENT_TIMER_100HZ,
	EVENT_TIMER_50HZ,
	EVENT_TIMER_20HZ,
	EVENT_TIMER_10HZ,
	EVENT_TIMER_5HZ,
	EVENT_TIMER_1HZ,
	
	EVENT_JOY_CLICK,
	EVENT_JOY_DOWN,
	EVENT_JOY_UP,
	EVENT_JOY_LEFT,
	EVENT_JOY_RIGHT,
	
	EVENT_START_GAME,
	EVENT_GOAL_SCORED,
} event_t;

uint8_t eq_initialize(void);
void eq_push_event(event_t pushed_event);
cb_pop_result_t eq_pop_next_event(event_t* next);

#endif /* EVENT_QUEUE_H_ */