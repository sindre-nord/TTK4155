/*
 * node2.c
 *
 * Created: 05.10.2023 10:47:18
 * Author : jknewall
 */ 

#include <stdbool.h>

#include "sam.h"
#include "sam3xa.h"

#include "../can_definitions/can_definitions.h"
#include "can_controller.h"
#include "can_interrupt.h"
#include "printf-stdarg.h"
#include "uart.h"
#include "motor.h"
#include "pwm.h"
#include "ir.h"



int main(void)
{
	WDT->WDT_MR = WDT_MR_WDDIS; //Disable watchdog timer
	PIOA->PIO_WPMR = ('P' << 24) + ('I' << 16) + ('O' << 8)+0; // Write protection magic word, see pg. 674 in datasheet
	
    /* Initialize the SAM system */
    SystemInit();
    configure_uart();
	pwm_init();
	ir_init();
	motor_init();
	//motor_enable();
	//motor_disable(); // TESTING! REMOVE TO WORK
	solenoid_init();
	can_init_def_tx_rx_mb(can_make_baudrate_config());

    /* Replace with your application code */
	bool is_in_goal = false;
	static const uint16_t ir_in_goal_min_threshold = 1000;
	static const uint16_t ir_out_of_goal_max_threshold = 3500;
    while (1) 
    {
		uint16_t ir_value = ir_read_value();
		if (is_in_goal && ir_value > ir_out_of_goal_max_threshold) {
			is_in_goal = false;
		} else if (!is_in_goal && ir_value < ir_in_goal_min_threshold) {
			is_in_goal = true;
			printf("GOOOAAAL\n\r");
			CAN_MESSAGE msg = {.id=CAN_ID_GOAL_SCORED, .data_length=sizeof(can_data_is_goal_t)};
			can_send(&msg, 0);
		}
		//printf("Current IR value: %u \r\n", ir_read_value());
    }
}
