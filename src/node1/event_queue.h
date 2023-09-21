/*
 * event.h
 *
 * Created: 19.09.2023 13:45:33
 *  Author: brageim
 */ 


#ifndef EVENT_QUEUE_H_
#define EVENT_QUEUE_H_

#include <stdint.h>

typedef enum{
	//Events
	EVENT_ENTRY,
	EVENT_EXIT,
	EVENT_JOY_CLICK,
	EVENT_JOY_DOWN,
	EVENT_JOY_UP,
} event_t;



#endif /* EVENT_QUEUE_H_ */