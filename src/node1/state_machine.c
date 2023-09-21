/*
 * state_machine.c
 *
 * Created: 19.09.2023 13:19:49
 *  Author: brageim
 */ 

#include "state_machine.h"


void fsm_initialize(fsm_t* fsm_p, fsm_state_fp initial_state){
	fsm_p->state = initial_state;
	fsm_dispatch(fsm_p, EVENT_ENTRY);
}

void fsm_dispatch(fsm_t* fsm_p, event_t current_event){
	(*fsm_p->state)(fsm_p, current_event);
}

void fsm_transition(fsm_t* fsm_p, fsm_state_fp next_state){
	fsm_dispatch(fsm_p, EVENT_EXIT);
	fsm_p->state = next_state;
	fsm_dispatch(fsm_p, EVENT_ENTRY);
}