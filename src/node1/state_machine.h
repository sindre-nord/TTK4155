/*
 * state_machine.h
 *
 * Created: 19.09.2023 13:20:01
 *  Author: brageim
 */ 
#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

#include "event_queue.h"

typedef struct fsm_t fsm_t;
typedef void (*fsm_state_fp)(fsm_t* fsm_p, event_t current_event);
struct fsm_t{
	fsm_state_fp state;
};

void fsm_initialize(fsm_t* fsm_p, fsm_state_fp initial_state);

void fsm_dispatch(fsm_t* fsm_p, event_t current_event);

void fsm_transition(fsm_t* fsm_p, fsm_state_fp next_state);

#endif /* STATE_MACHINE_H_ */