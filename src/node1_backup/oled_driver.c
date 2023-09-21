/*
 * oled_driver.c
 *
 * Created: 18.09.2023 10:28:03
 *  Author: jknewall
 */ 

#include "oled_driver.h"

#include <string.h>

#include "fonts.h"

//static unsigned char* const oled_base_address = ;
//static unsigned char* const oled_pixel_address = oled_base_address;
#define OLED_COMMAND_PIN 8 // Address pin number used for command. Active low


typedef struct {
	unsigned char command[1<<OLED_COMMAND_PIN];
	oled_segment_t next_segment;
} oled_map_t;

static volatile oled_map_t* oled = (oled_map_t*)0x1200;

void oled_init(void) {
	oled_write_command(OLED_DISPLAY_OFF);
	oled_write_command(OLED_SEGMENT_REMAP);
	oled_write_command_value(OLED_COMMON_PADS_HARDWARE_ALTERNATIVE, 0x12);
	oled_write_command(OLED_COMMON_OUTPUT_SCAN_DIRECTION);
	oled_write_command_value(OLED_MULTIPLEX_RATION_MODE, 63);
	oled_write_command_value(OLED_DISPLAY_DIVIDE_RATIO, 0x80);
	oled_write_command_value(OLED_CONTRAST_CONTROL, 0x50);
	oled_write_command_value(OLED_SET_PRECHARGE_PERIOD, 0x21);
	oled_write_command_value(OLED_SET_MEMORY_ADDRESSING_MODE, 0x02);
	oled_write_command_value(OLED_VCOM_DESELECT_LEVEL_MODE, 0x30);
	oled_write_command_value(OLED_MASTER_CONFIGURATION, 0x00);
	oled_write_command(OLED_OUT_FOLLOWS_RAM_CONTENT);
	oled_write_command(OLED_SET_NORMAL_DISPLAY);
	oled_write_command(OLED_DISPLAY_ON);
}

void oled_write_command(oled_command_t command){
	*oled->command = command;
}

void oled_write_command_value(oled_command_t command, uint8_t value){
	*oled->command = command;
	*oled->command = value;
}

void oled_select_segment(uint8_t page, uint8_t column) {
	oled_select_page(page);
	oled_select_column(column);
}

void oled_select_page(uint8_t page) {
	oled_write_command(OLED_SET_PAGE | page);
}

void oled_select_column(uint8_t column) {
	oled_write_command(OLED_SET_LOWER_COLUMN_ADDR | (column & 0b1111));
	oled_write_command(OLED_SET_HIGHER_COLUMN_ADDR | (column >> 4));
}

void oled_write_segment(oled_segment_t value) {
	oled->next_segment = value;
}

void oled_clear(void) {
	for (uint8_t page = 0; page < OLED_NUM_PAGES; page++) {
		oled_select_segment(page, 0);
		for (uint8_t segment = 0; segment < OLED_NUM_SEGMENTS_PER_PAGE; segment++) {
			oled_write_segment(0x00);
		}
	}
}

int oled_putchar(char c, FILE* stream) {
	if (FONT_START_ASCII <= c && c <= FONT_END_ASCII) {
		for (uint8_t i = 0; i < FONT_WIDTH_MEDIUM; i++) {
			oled_write_segment(pgm_read_byte(&(font_medium[c-FONT_START_ASCII][i])));
		}
		oled_write_segment(0);
	}
	return 0;
}

void oled_print(char* str) {
	printf(str);
}