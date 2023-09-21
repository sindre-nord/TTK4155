/*
 * adc_driver.h
 *
 * Created: 11.09.2023 13:54:06
 *  Author: jknewall
 */ 


#ifndef ADC_DRIVER_H_
#define ADC_DRIVER_H_

#include <stdint.h>

void adc_init(void);
uint8_t adc_read(uint8_t channel);




#endif /* ADC_DRIVER_H_ */