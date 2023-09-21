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

#define MENU_MAX_ENTRIES 8
#define MENU_ENTRY_MAX_CHARACTERS 20
typedef struct menu menu_t;



//typedef char[MENU_MAX_ENTRIES][32] display_text_t;

typedef struct {
	unsigned char display_text[MENU_ENTRY_MAX_CHARACTERS];
} menu_entry_t;

typedef struct menu {
	//menu_t* parent_menu;
	uint8_t num_entries;
	uint8_t selected_entry;
	//display_text_t* display_text_ptr;
	menu_entry_t entries[MENU_MAX_ENTRIES];
} menu_t;

void menu_render_cursor(void);
void menu_clear_cursor(void);
void menu_render(menu_t* menu_p);

#endif /* MENU_H_ */