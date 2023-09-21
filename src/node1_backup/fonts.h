/*
 * fonts.h
 *
 * Library of fonts
 * Large: 8x8, normal: 5x7 and small: 4x7
 */
#ifndef FONTS_H_
#define FONTS_H_


#include <avr/pgmspace.h>

enum {
	FONT_WIDTH_LARGE = 8,
	FONT_WIDTH_MEDIUM = 5,
	FONT_WIDTH_SMALL = 4,
	FONT_START_ASCII = 32,	// Font starts at this ascii value
	FONT_END_ASCII = 126	// ..and ends at this
};

// Font 8x8 - Large
extern const unsigned char font_large[95][FONT_WIDTH_LARGE];

// Font 5x7 - normal
extern const unsigned char font_medium[95][FONT_WIDTH_MEDIUM];

// Font 4x6 - Small
extern const unsigned char font_small[95][FONT_WIDTH_SMALL];


#endif /* FONTS_H_ */