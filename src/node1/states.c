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
menu_t main_menu;
static menu_t play = (menu_t){
	.num_entries=2, .selected_entry=0,
	{
		{menu_text[5], NULL, &main_menu},
		{menu_text[6], NULL, &main_menu},
	}
};

static menu_t highscore = (menu_t){
	.num_entries=2, .selected_entry=0,
	{
		{menu_text[7], NULL, &main_menu},
		{menu_text[8], NULL, &main_menu},
	}
};
menu_t main_menu = (menu_t){
	.num_entries=6, .selected_entry=0,
	{
		{menu_text[0], &play},
		{menu_text[1], &highscore},
		{menu_text[2], NULL},
		{menu_text[3], NULL},
		{menu_text[4], NULL},

		// {"Submenu1", NULL},
		// {"Submenu2", NULL},
		// {"Submenu3", NULL},
		// {"Submenu4", &submenu_1},
		// {"Submenu5", &submenu_2},
		// {"Submenu6", NULL},
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
			if(selected_menu->selected_entry < selected_menu->num_entries-2){
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
			}
			menu_render(selected_menu);
			break;
		case EVENT_JOY_LEFT:
			if(selected_menu->entries[selected_menu->selected_entry].parent_menu != NULL){
				selected_menu = selected_menu->entries[selected_menu->selected_entry].parent_menu;
			}
			menu_render(selected_menu);
			break;
		case EVENT_EXIT:
			break;
		
		//case "something triggering state change":
			//fsm_transition(fsm_p, fsm_state_something_else);
		default:
			break;
	}
}