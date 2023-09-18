/*
 * oled_driver.h
 *
 * Created: 18.09.2023 10:28:18
 *  Author: jknewall
 */ 


#ifndef OLED_DRIVER_H_
#define OLED_DRIVER_H_

#include <stdio.h>

#define OLED_NUM_PAGES 8
#define OLED_NUM_SEGMENTS_PER_PAGE 128

// Hexadecimal definitions taken directly from OLED datasheet recommended init, binary values from OLED controller datasheet table 8-1
typedef enum {
	OLED_NO_OPERATION = 0b11100011,
	OLED_DISPLAY_OFF = 0xae,
	OLED_DISPLAY_ON = 0xaf,
	OLED_ENTIRE_DISPLAY_ON = 0b10100101,
	OLED_SET_LOWER_COLUMN_ADDR = 0b00000000,
	OLED_SET_HIGHER_COLUMN_ADDR = 0b00010000,
	OLED_SET_PAGE = 0b10110000,
	OLED_SEGMENT_REMAP = 0xa1,
	OLED_COMMON_PADS_HARDWARE_ALTERNATIVE = 0xda,
	OLED_COMMON_OUTPUT_SCAN_DIRECTION = 0xc8,
	OLED_MULTIPLEX_RATION_MODE = 0xa8,
	OLED_DISPLAY_DIVIDE_RATIO = 0xd5,
	OLED_CONTRAST_CONTROL = 0x81,
	OLED_SET_PRECHARGE_PERIOD = 0xd9,
	OLED_SET_MEMORY_ADDRESSING_MODE = 0x20,
	OLED_VCOM_DESELECT_LEVEL_MODE = 0xdb,
	OLED_MASTER_CONFIGURATION = 0xad,
	OLED_OUT_FOLLOWS_RAM_CONTENT = 0xa4,
	OLED_SET_NORMAL_DISPLAY = 0xa6,
} oled_command_t;

void oled_init(void);
void oled_write_command(oled_command_t command);
void oled_write_command_value(oled_command_t command, uint8_t value);
void oled_select_segment(uint8_t page, uint8_t column);
void oled_select_page(uint8_t page);
void oled_select_column(uint8_t column);

typedef unsigned char oled_segment_t;
void oled_write_segment(oled_segment_t value);


#endif /* OLED_DRIVER_H_ */