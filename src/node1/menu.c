/*
 * menu_library.c
 *
 * Created: 19.09.2023 14:11:32
 *  Author: brageim
 */ 

#include "menu.h"

#include <stdio.h>

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
	stdout = fdevopen(oled_putchar, NULL);
	for (uint8_t entry = 0; entry < menu_p->num_entries; entry++) {
		oled_select_segment(entry, 0);
		if (entry == menu_p->selected_entry) {
			menu_render_cursor();
		} else {
			menu_clear_cursor();
		}
		printf(menu_p->entries[entry].display_text);
	}
	stdout = prior_stdout;
}