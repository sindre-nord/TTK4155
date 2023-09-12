/*
 * adc_driver.c
 *
 * Created: 11.09.2023 13:54:22
 *  Author: jknewall
 */ 
#include "adc_driver.h"

#include <avr/io.h>
#include <stdio.h>

// Based on table 1 ("Multiplexer configurations") in MAX156 datasheet
typedef enum {
	ADC_A0,
	ADC_A1,
	ADC_UNUSED,
	ADC_POWER_DOWN,
	ADC_INHIBIT_CONVERSION,
	ADC_BIPOLAR, // opposite: unipolar
	ADC_DIFFERENTIAL, // opposite: single-ended
	ADC_CONVERT_SINGLE // opposite: convert all previously configured
} adc_config_t;

static volatile unsigned char* const adc_base_address = (unsigned char*)0x1400;

void adc_init(void){
	static const uint8_t shared_channel_config = (1<<ADC_CONVERT_SINGLE)|(1<<ADC_INHIBIT_CONVERSION);

	for (uint8_t channel = 0; channel < 4; channel++) {
		adc_base_address[channel] = shared_channel_config | channel;
	}
	
	DDRE &= ~(1<<DDE0); // Connected to active low ADC BUSY signal
}

uint8_t adc_read(uint8_t channel) {
	adc_base_address[channel] = (1<<ADC_CONVERT_SINGLE) | channel;
	while(!(PINE & 1<<PINE0)){PINE;/*Arbitrary instruction needed for the AVR to actually wait for the busy pin*/}
	return adc_base_address[channel];
}
