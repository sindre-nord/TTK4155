/*
 * menu_library.c
 *
 * Created: 19.09.2023 14:11:32
 *  Author: brageim
 */ 

#include "menu.h"

#include <stdio.h>

#include "event_queue.h"
#include "oled_driver.h"

void menu_render_cursor(void) {
	oled_write_segment(0b00011000);
	oled_write_segment(0b00011000);
	oled_write_segment(0b01111110);
	oled_write_segment(0b00111100);
	oled_write_segment(0b00011000);
	oled_write_segment(0b00000000);
}

void menu_clear_cursor(void) {
	oled_write_segment(0b00000000);
	oled_write_segment(0b00000000);
	oled_write_segment(0b00000000);
	oled_write_segment(0b00000000);
	oled_write_segment(0b00000000);
	oled_write_segment(0b00000000);
}

#include "uart_driver.h"

void menu_render(menu_t* menu_p) {
	oled_clear();
	FILE* prior_stdout = stdout;
	stdout = oled_stdout;
	for (uint8_t entry = 0; entry < menu_p->num_entries; entry++) {
		char entry_text[MENU_ENTRY_MAX_CHARACTERS];
		oled_select_segment(entry, 0);
		if (entry == menu_p->selected_entry) {
			menu_render_cursor();
		} else {
			menu_clear_cursor();
		}
		memccpy_P(entry_text, &(menu_p->display_text[0][entry]), '\0', MENU_ENTRY_MAX_CHARACTERS);
		printf(entry_text);
	}
	stdout = prior_stdout;
}

const unsigned char PROGMEM menu_text[MAX_AMOUNT_OF_MENUS][MENU_MAX_ENTRIES][MENU_ENTRY_MAX_CHARACTERS] = {
	[MENU_TEXT_MAIN] = {
		"Play",
		"Highscore",
		"Settings",
		"Credits",
		"Exit",
	},
// 	[MENU_TEXT_PLAY] = {
// 		"Play as Kalle",
// 		"Play as Sindre",
// 	},
	[MENU_TEXT_HIGHSCORE] = {
		"Kalle: 2000",
		"Sindre: 9000",
	},
	[MENU_TEXT_CREDITS] = {
		"Kalle",
		"Sindre",
		"et. al.",
	},
};

static menu_t menu_main, /*menu_play,*/ menu_highscore, menu_credits;
static menu_t menu_main = {
	.num_entries=5, .selected_entry=0, .parent_menu = NULL, .display_text=&menu_text[MENU_TEXT_MAIN],
	{
		{NULL, EVENT_START_GAME},
		{&menu_highscore,},
		{NULL,},
		{&menu_credits,},
		{NULL,},
	}
};
// static menu_t menu_play = {
// 	.num_entries=2, .selected_entry=0, .parent_menu=&menu_main, .display_text=&menu_text[MENU_TEXT_PLAY],
// 	{
// 		{NULL,},
// 		{NULL,},
// 	}
// };
static menu_t menu_highscore = {
	.num_entries=2, .selected_entry=0, .parent_menu=&menu_main, .display_text=&menu_text[MENU_TEXT_HIGHSCORE],
	{
		{NULL,},
		{NULL,},
	}
};
static menu_t menu_credits = {
	.num_entries=3, .selected_entry=0, .parent_menu=&menu_main, .display_text=&menu_text[MENU_TEXT_CREDITS],
	{
		{NULL,},
		{NULL,},
		{NULL,},
	}
};

menu_t* selected_menu = &menu_main;
