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
void state_menu(fsm_t* fsm_p, event_t current_event) {
	
	static menu_t main_menu;
	static menu_t submenu1;
	static menu_t submenu2;
	static menu_t submenu3;
	// address 0x800594 of node1.elf section `.bss' is not within region `data'	node1
	main_menu = (menu_t){
		.parent_menu=NULL, .num_entries=2, .selected_entry=0,
		{
			{"Submenu1", NULL, NULL},
			{"Submenu2", &submenu3, NULL}
		}
	};
	submenu1 = (menu_t){
		.parent_menu=&submenu2, .num_entries=2, .selected_entry=0,
		{
			{"Menu 1", NULL, NULL},
			{"Menu 1", NULL, NULL}
		}
	};
	submenu2 = (menu_t){
		.parent_menu=&submenu1, .num_entries=2, .selected_entry=0,
		{
			{"Menu 1", NULL, NULL},
			{"Menu 1", NULL, NULL}
		}
	};
	
	//menu_render(&main_menu);
	printf("states print");
	
	switch(current_event){
		
		case EVENT_ENTRY:
		
			break;
		case EVENT_EXIT:
			
			break;
			
		//case "something triggering state change":
			//fsm_transition(fsm_p, fsm_state_something_else);
		default:
			break;
	}
}