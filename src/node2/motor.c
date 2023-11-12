/*
 * motor.c
 *
 * Created: 23.10.2023 14:28:37
 *  Author: jknewall
 */ 
#include "motor.h"
#include "sam.h"
#include <stdint.h>
#include <stdio.h>

// typedef enum{
// 	MOTOR_ENABLE = PIO_SODR_P9,
// 	MOTOR_DISABLE = PIO_CODR_P9, 
// 	ENCODER_SELECT_LOW_BYTE = PIO_SODR_P2,
// 	ENCODER_SELECT_HIGH_BYTE = PIO_CODR_P2,
// 	MOTOR_DIRECTION_RIGHT = PIO_SODR_P10,
// 	MOTOR_DIRECTION_LEFT = PIO_CODR_P10,
// 	MOTOR_RESET = PIO_SODR_P1, // Active low
// 	MOTOR_OUTPUT_ENABLE = PIO_SODR_P0, // Active low
// };

void delay_20us(void) {
    for(uint16_t i = 0; i < 1680; i++) {
        asm volatile ("nop");
    }
}

void motor_init(void){
	PMC->PMC_PCER0 |= 1 << ID_PIOC;
	//Set data direction for MJ1 pins
	PIOD->PIO_OER |= PIO_OER_P10; //Motor direction pin
	PIOD->PIO_OER |= PIO_OER_P9; //Motor enable
	PIOD->PIO_OER |= PIO_OER_P2; //Motor select
	PIOD->PIO_OER |= PIO_OER_P1; //!Reset
	PIOD->PIO_OER |= PIO_OER_P0; //!Output enable
	
	PIOD->PIO_SODR |= PIO_SODR_P1;
	
	PIOC->PIO_PER |= PIO_PER_P1;
	PIOC->PIO_PER |= PIO_PER_P2;
	PIOC->PIO_PER |= PIO_PER_P3;
	PIOC->PIO_PER |= PIO_PER_P4;
	PIOC->PIO_PER |= PIO_PER_P5;
	PIOC->PIO_PER |= PIO_PER_P6;
	PIOC->PIO_PER |= PIO_PER_P7;
	PIOC->PIO_PER |= PIO_PER_P8;

	dac_init();
}

void motor_set_speed(int16_t speed){
	if(speed < 0){
		PIOD->PIO_SODR |= PIO_SODR_P10; // Set direction
		dac_set_value((uint16_t)-speed);
	}else{
		PIOD->PIO_CODR |= PIO_CODR_P10; // Set direction
		dac_set_value((uint16_t)speed);
	}
}

static float motor_controller_integral;

void motor_controller_reset(void){
	motor_controller_integral = 0;
}

// Should normalize all interpretations of position to [0, 1] (-Anders)

void motor_control_position(uint16_t pos) {
	//pos = 0.5;
	static float KI = 0.1;
	static float KP = 20;
	uint16_t motor_position = motor_read_position();
	//printf("Motor position: %u \n\r", (motor_position*255/312));
		
	//float normalized_motor_position = (float)(motor_position/312);
 	//float error = pos - (normalized_motor_position);
	float error = (float)(pos - (motor_position*255/312));
	 
	motor_controller_integral += error;
  	int16_t speed = (int16_t)(error*KP+motor_controller_integral*KI);
	//printf("Speed: %d \n\r", speed);
	motor_set_speed(speed);
}

void motor_enable(void){
	PIOD->PIO_SODR |= PIO_SODR_P9;
}

void motor_disable(void){
	PIOD->PIO_CODR |= PIO_CODR_P9;
}

void encoder_calibrate(void){
	PIOD->PIO_CODR |= PIO_CODR_P1;
	delay_20us();
	//delay_20us();
	PIOD->PIO_SODR |= PIO_SODR_P1;
}

// Reads the selected byte
uint8_t encoder_read_byte(void){
	// Read out the byte from port c pin 1 to 8
    // Mask for pins P1 through P8
    const uint32_t PIN_MASK = PIO_PDSR_P1 | PIO_PDSR_P2 | PIO_PDSR_P3 | PIO_PDSR_P4 |
                              PIO_PDSR_P5 | PIO_PDSR_P6 | PIO_PDSR_P7 | PIO_PDSR_P8;
    
    // Read port and mask out the relevant pins
    uint32_t port_data = PIOC->PIO_PDSR & PIN_MASK;

    // Right-shift the data to get the byte value (assuming P1 is LSB)
    return (uint8_t)(port_data >> 1);
}
	


uint16_t motor_read_position(void){
	uint16_t motor_position = 0;
	
	PIOD->PIO_CODR = PIO_CODR_P0; // Set output enable low to begin read 
	PIOD->PIO_CODR = PIO_CODR_P2; // Set encoder select low to output high byte
	// Wait 20 microseconds
	delay_20us();
	
	motor_position |= ((uint16_t)encoder_read_byte()) << 8; // Read high byte into motor position
	//printf("Motor position high byte: %u \n\r", motor_position);
	PIOD->PIO_SODR |= PIO_SODR_P2; // Set encoder select high to output low byte
	// Wait 20 microseconds
	delay_20us();

	motor_position |= (uint16_t)encoder_read_byte(); // Read low byte into motor position
	PIOD->PIO_SODR |= PIO_SODR_P0; // Set output enable high to end read 
	return motor_position;	
}

void dac_init(void){
	PMC->PMC_PCER1 |= PMC_PCER1_PID38;
	DACC->DACC_IER |= DACC_IER_TXRDY;
	DACC->DACC_MR |= DACC_MR_USER_SEL_CHANNEL1;// | (2 << DACC_MR_REFRESH_Pos);
	DACC->DACC_CHER |= DACC_CHER_CH1;
}

void dac_set_value(uint16_t value){
	while (!(DACC->DACC_ISR | DACC_ISR_TXRDY)) {;}
	DACC->DACC_CDR = value;
}


void solenoid_init(void){
	PIOC->PIO_OER |= PIO_OER_P12; // Pin direction
}

void solenoid_enable(void){
	PIOC->PIO_SODR |= PIO_SODR_P12; // Pin 51 on ATSAM
}

void solenoid_disable(void){
	PIOC->PIO_CODR |= PIO_CODR_P12;
}

