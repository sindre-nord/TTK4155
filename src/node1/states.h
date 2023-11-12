/*
 * state.h
 *
 * Created: 19.09.2023 14:01:59
 *  Author: brageim
 */ 


#ifndef STATE_H_
#define STATE_H_

#include "event_queue.h"
#include "state_machine.h"


void state_menu(fsm_t* fsm_p, event_t current_event);
void state_game_startup(fsm_t* fsm_p, event_t current_event);
void state_game(fsm_t* fsm_p, event_t current_event);
void state_game_over(fsm_t* fsm_p, event_t current_event);

#endif /* STATE_H_ */