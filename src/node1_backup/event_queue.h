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
} event_t;



#endif /* EVENT_QUEUE_H_ */