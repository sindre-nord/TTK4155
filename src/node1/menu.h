/*
 * menu_lib.h
 *
 * Created: 19.09.2023 14:11:19
 *  Author: brageim
 */ 


#ifndef MENU_H_
#define MENU_H_

#include <avr/pgmspace.h>
#include <stddef.h>
#include <stdint.h>

#include "event_queue.h"

#define MENU_MAX_ENTRIES 8
#define MENU_ENTRY_MAX_CHARACTERS 20
#define MAX_AMOUNT_OF_MENUS 20
typedef struct menu menu_t;

typedef enum {
	MENU_TEXT_MAIN,
	MENU_TEXT_PLAY,
	MENU_TEXT_HIGHSCORE,
	MENU_TEXT_CREDITS
} menu_text_t;

//extern const unsigned char menu_text[20][MENU_ENTRY_MAX_CHARACTERS];
extern const unsigned char menu_text[MAX_AMOUNT_OF_MENUS][MENU_MAX_ENTRIES][MENU_ENTRY_MAX_CHARACTERS];
extern menu_t* selected_menu;

typedef struct {
	menu_t* next_menu;
	event_t associated_event;
} menu_entry_t;

typedef struct menu {
	uint8_t num_entries;
	uint8_t selected_entry;
	menu_t* parent_menu;
	const unsigned char (*display_text)[MENU_MAX_ENTRIES][MENU_ENTRY_MAX_CHARACTERS];
	menu_entry_t entries[MENU_MAX_ENTRIES];
} menu_t;

void menu_render_cursor(void);
void menu_clear_cursor(void);
void menu_render(menu_t* menu_p);

#endif /* MENU_H_ */