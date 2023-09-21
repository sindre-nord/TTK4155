/*
 * states.c
 *
 * Created: 19.09.2023 14:01:43
 *  Author: brageim
 */ 

#include "states.h"

#include <stdint.h>

#include "menu.h"

#include <stdio.h>


// address 0x800594 of node1.elf section `.bss' is not within region `data'	node1
static menu_t main_menu = (menu_t){
	.num_entries=8, .selected_entry=0,
	{
		{"Submenu1"},
		{"Submenu2"},
		{"Submenu3"},
		{"Submenu4"},
		{"Submenu5"},
		{"Submenu6"},
		{"Submenu7"},
		{"Submenu8"},
	}
};
menu_t* selected_menu = &main_menu;


void state_menu(fsm_t* fsm_p, event_t current_event) {
	
	//printf("states print");
	switch(current_event){
		
		case EVENT_ENTRY:
			menu_render(selected_menu);
			break;
			
		case EVENT_JOY_DOWN:
			selected_menu->selected_entry++;
			menu_render(selected_menu);
			break;
		
		case EVENT_JOY_CLICK:
			break;
			
		case EVENT_EXIT:
			break;
		
		//case "something triggering state change":
			//fsm_transition(fsm_p, fsm_state_something_else);
		default:
			break;
	}
}