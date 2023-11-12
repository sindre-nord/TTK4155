/*
 * states.c
 *
 * Created: 19.09.2023 14:01:43
 *  Author: brageim
 */ 

#include "states.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../can_definitions/can_definitions.h"
#include "can.h"
#include "player_input_driver.h"
#include "menu.h"
#include "oled_driver.h"
#include "timer.h"


void state_menu(fsm_t* fsm_p, event_t current_event) {
	switch(current_event){
		
		case EVENT_ENTRY:
			printf("Entering menu\n");
			menu_render(selected_menu);
			break;
			
		case EVENT_JOY_DOWN:
			if(selected_menu->selected_entry < selected_menu->num_entries-1){
				selected_menu->selected_entry++;
			}
			menu_render(selected_menu);
			break;
		case EVENT_JOY_UP:
			if(selected_menu->selected_entry > 0){
				selected_menu->selected_entry--;
			}
			menu_render(selected_menu);
			break;
		case EVENT_JOY_RIGHT:
			if(selected_menu->entries[selected_menu->selected_entry].next_menu != NULL){
				selected_menu = selected_menu->entries[selected_menu->selected_entry].next_menu;
				menu_render(selected_menu);
			}
			if(selected_menu->entries[selected_menu->selected_entry].associated_event != EVENT_NULL) {
				eq_push_event(selected_menu->entries[selected_menu->selected_entry].associated_event);
			}
			break;
		case EVENT_JOY_LEFT:
			if(selected_menu->parent_menu != NULL){
				selected_menu = selected_menu->parent_menu;
			}
			menu_render(selected_menu);
			break;
			
		case EVENT_START_GAME:
			fsm_transition(fsm_p, state_game_startup);
			break;
			
		case EVENT_EXIT:
			break;

		default:
			break;
	}
}

void state_game_startup(fsm_t* fsm_p, event_t current_event) {
	static uint8_t counter;
	switch(current_event){
		
		case EVENT_ENTRY: {
			oled_clear();
			printf("Starting game\n");
			counter = 0;
			static can_frame_t frame = {.id=CAN_ID_MOTOR_TO_START, .data_length=0};
			can_send(&frame);
			break;
		}

		case EVENT_TIMER_100HZ: {
			if (counter++ == 100) {
				fsm_transition(fsm_p, state_game);
				static can_frame_t frame = {.id=CAN_ID_CALIBRATE_ENCODER, .data_length=0};
				can_send(&frame);
			}
			break;
		}

		case EVENT_EXIT:
		break;

		default:
		break;
	}
}

static uint16_t score;
static uint16_t highscore;

void state_game(fsm_t* fsm_p, event_t current_event) {
	switch(current_event){
		
		case EVENT_ENTRY:
			oled_clear();
			printf("Started game\n");
			score = 0;
			break;

		case EVENT_TIMER_100HZ: {
			score++;
			static can_frame_t frame = {.id=CAN_ID_JOYSTICK_STATE, .data_length=sizeof(can_data_gamepad_state_t)};
			can_data_gamepad_state_t joy_stick_state = {
				.stick_horizontal_pos=player_input.joy_stick[JOY_HORIZONTAL],
				.stick_vertical_pos=player_input.joy_stick[JOY_VERTICAL],
				.slider_pos=player_input.slider[SLIDER_RIGHT],
				.button_state=read_right_button()
			};
			memcpy(&frame.data, &joy_stick_state, frame.data_length);
			can_send(&frame);
			break;
		}
		
		
		
		case EVENT_GOAL_SCORED:
			printf("Loser! Score: %u\n\r", score);
			fsm_transition(fsm_p, state_game_over);
			break;
		
		case EVENT_JOY_UP:
			fsm_transition(fsm_p, state_menu);
			break;

		case EVENT_EXIT: {
			static can_frame_t frame = {.id=CAN_ID_MOTOR_DISABLE, .data_length=0};
			can_send(&frame);
			break;
		}
		default:
			break;
	}
}

void state_game_over(fsm_t* fsm_p, event_t current_event){
	switch(current_event){
		case EVENT_ENTRY:
		if(score > highscore){
			highscore = score;
		}
		oled_clear();
		FILE* prior_stdout = stdout;
		stdout = oled_stdout;
		oled_select_segment(0, 0);
		printf("Game Over!");
		oled_select_segment(1, 0);
		printf("Score: %u", score);
		oled_select_segment(2, 0);
		printf("Highscore: %u", highscore);
		oled_select_segment(4, 0);
		printf("Move joystick");
		oled_select_segment(5, 0);
		printf("to continue...");
		stdout = prior_stdout;
		break;
		
		case EVENT_JOY_LEFT:
		case EVENT_JOY_UP:
		case EVENT_JOY_RIGHT:
		case EVENT_JOY_DOWN:
		fsm_transition(fsm_p, state_menu);
		break;
		
		default:
			break;
	}
	
}