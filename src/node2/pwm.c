/*
 * pwm.c
 *
 * Created: 16.10.2023 15:50:58
 *  Author: jknewall
 */ 

#include "pwm.h"
#include "sam.h"
#include <assert.h>

#define AT_SAM_MCK_FREQUENCY (84000000ULL)
#define PWM_CHANNEL_PERIOD 0.02 //[seconds]


void pwm_init(void){
	PMC->PMC_PCER1 |= 1<<(ID_PWM-32); //Enable PWM clock
	PIOC->PIO_OER |= PIO_PC19;
	PIOC->PIO_PDR |= PIO_PDR_P19; //Disable IO on pin n
	PIOC->PIO_ABSR |= PIO_ABSR_P19; //Enable peripheral on pin n

	//REG_PWM_CLK = 0b101011111111; //PWM clock = MCK/1024
	
	REG_PWM_CMR5 = PWM_CMR_CPRE_MCK_DIV_1024 | PWM_CMR_CPOL; //Use MCK/1024 PWM and start high
	
	REG_PWM_CPRD5 = (uint32_t)((PWM_CHANNEL_PERIOD*AT_SAM_MCK_FREQUENCY)/1024); //Set period
	
	pwm_set_angle(90);
	REG_PWM_ENA = PWM_ENA_CHID5;
	
// 	PIOA->PIO_PER |= PIO_PER_P19; //PIO enable on pin 19
// 	PMC->PMC_PCER0 |= PMC_PCER0_PID11; //Peripheral clock, don't know which bit to set
// 	
// 	PIOA->PIO_OER |= PIO_OER_P19; //Enable output on pin 19
// 	PIOA->PIO_SODR |= PIO_SODR_P19; //Set data on pin 19
}

void pwm_set_angle(uint16_t angle){
	if(angle > 180){
		angle = 180;
	}
	REG_PWM_CDTY5 = (uint32_t)(REG_PWM_CPRD5*(0.05+0.05*(180-angle)/180));
}