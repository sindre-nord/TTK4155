/*
 * ir.c
 *
 * Created: 23.10.2023 11:44:42
 *  Author: jknewall
 */ 

#include "ir.h"
#include "sam.h"

void ir_init(void) {
	PMC->PMC_PCER1 |= PMC_PCER1_PID37;
	ADC->ADC_MR = ADC_MR_FREERUN_ON;
	ADC->ADC_CHER = ADC_CHER_CH0;
	ADC->ADC_CR = ADC_CR_START;
	
}

uint16_t ir_read_value(void){
	return ADC->ADC_LCDR & 0b111111111111;
}