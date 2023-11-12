/*
 * can_controller.c
 *
 * Author: Gustav O. Often and Eivind H. J�lsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */ 

#include "can_controller.h"

#include <string.h>
#include "sam.h"
#include "printf-stdarg.h"

#include "../can_definitions/can_definitions.h"
#include "motor.h"
#include "pwm.h"

#define AT_SAM_CAN_BAUD_RATE 19200ULL

#define AT_SAM_MCK_FREQUENCY (84000000ULL/4) // CAN controller divides system clock (84MHz for Arduino Due) by 4
#define AT_SAM_SJW 0 // Sync = (SJW + 1) x T_Q  (Sync = Synchronization Jump Width Length)
#define AT_SAM_PRSEG 2 // PropSeg = (PRSEG + 1) x TQ
#define AT_SAM_PHSEG1 2 // PS1 = (PHSEG1 + 1) x TQ
#define AT_SAM_PHSEG2 2 // PS2 = (PHSEG2 + 1) x TQ
#define AT_SAM_BRP (uint8_t)(AT_SAM_MCK_FREQUENCY/(AT_SAM_CAN_BAUD_RATE*(unsigned long long)(AT_SAM_PRSEG+AT_SAM_PHSEG1+AT_SAM_PHSEG2+4)) - 1)

uint32_t can_make_baudrate_config(void) {
	printf("Baudrate prescaler: %u \n\r", AT_SAM_BRP);
	uint32_t can_baudrate_register = 0;
	can_baudrate_register |= AT_SAM_BRP << CAN_BR_BRP_Pos;
	can_baudrate_register |= AT_SAM_SJW << CAN_BR_SJW_Pos;
	can_baudrate_register |= AT_SAM_PRSEG << CAN_BR_PROPAG_Pos;
	can_baudrate_register |= AT_SAM_PHSEG1 << CAN_BR_PHASE1_Pos;
	can_baudrate_register |= AT_SAM_PHSEG2 << CAN_BR_PHASE2_Pos;
	return can_baudrate_register;
}

/**
 * \brief Initialize can bus with predefined number of rx and tx mailboxes, 
 * CAN0->CAN_MB[0] is used for transmitting
 * CAN0->CAN_MB[1,2] is used for receiving
 *			
 * \param can_br Value to be set in CAN0->CAN_BR register to match can bus bit timing
 *
 * \retval Success(0) or failure(1)
 */
uint8_t can_init_def_tx_rx_mb(uint32_t can_br)
{
	return can_init(can_br, 1, 2);
}

/**
 * \brief Initialize can bus
 *
 * \param can_br Value to be set in CAN0->CAN_BR register to match can bus bit timing
 *
 * \param num_tx_mb Number of transmit mailboxes, 	tx mb indexes: [0		, num_tx_mb-1]
 *
 * \param num_rx_mb Number of receive mailboxes, 	rx mb indexes: [num_tx_mb, num_rx_mb-1]
 *
 * \retval Success(0) or failure(1)
 */


uint8_t can_init(uint32_t can_br, uint8_t num_tx_mb, uint8_t num_rx_mb)
{
	
	//Make sure num_rx_mb and num_tx_mb is valid
	if((num_rx_mb > 8) | (num_tx_mb > 8) | (num_rx_mb + num_tx_mb > 8))
	{
		return 1; //Too many mailboxes is configured
	}


	uint32_t ul_status; 
	
	//Disable can
	CAN0->CAN_MR &= ~CAN_MR_CANEN; 
	//Clear status register on read
	ul_status = CAN0->CAN_SR; 
	(void)ul_status;
	
	
	// Disable interrupts on CANH and CANL pins
	PIOA->PIO_IDR = PIO_PA8A_URXD | PIO_PA9A_UTXD;
	
	//Select CAN0 RX and TX in PIOA
	uint32_t ul_sr = PIOA->PIO_ABSR;
	PIOA->PIO_ABSR = ~(PIO_PA1A_CANRX0 | PIO_PA0A_CANTX0) & ul_sr;
	
	// Disable the Parallel IO (PIO) of the Rx and Tx pins so that the peripheral controller can use them
	PIOA->PIO_PDR = PIO_PA1A_CANRX0 | PIO_PA0A_CANTX0;
	
	// Enable pull up on CANH and CANL pin
	PIOA->PIO_PUER = (PIO_PA1A_CANRX0 | PIO_PA0A_CANTX0);
	
	
	//Enable Clock for CAN0 in PMC
	PMC->PMC_PCR = PMC_PCR_EN | (2 << PMC_PCR_DIV_Pos) | PMC_PCR_CMD | (ID_CAN0 << PMC_PCR_PID_Pos); // DIV = 1(can clk = MCK/2), CMD = 1 (write), PID = 2B (CAN0)
	PMC->PMC_PCER1 |= 1 << (ID_CAN0 - 32);
	
	//Set baudrate, Phase1, phase2 and propagation delay for can bus. Must match on all nodes!
	CAN0->CAN_BR = can_br;
	

	/****** Start of mailbox configuration ******/

	uint32_t can_ier = 0;

	/* Configure receive mailboxes */
	for (int n = num_tx_mb; n <= num_rx_mb + num_tx_mb; n++)  //Simply one mailbox setup for all messages. You might want to apply filter for them.
	{
		CAN0->CAN_MB[n].CAN_MAM = 0; //Accept all messages
		CAN0->CAN_MB[n].CAN_MID = (CAN_MID_MIDE) | ((n+1) << CAN_MAM_MIDvA_Pos);
		CAN0->CAN_MB[n].CAN_MMR = (CAN_MMR_MOT_MB_RX);
		CAN0->CAN_MB[n].CAN_MCR |= CAN_MCR_MTCR;

		can_ier |= 1 << n; //Enable interrupt on rx mailbox
	}
	
	/*Configure transmit mailboxes */
	for (int n = 0; n < num_tx_mb; n++)
	{
		CAN0->CAN_MB[n].CAN_MID = CAN_MID_MIDE;
		CAN0->CAN_MB[n].CAN_MMR = (CAN_MMR_MOT_MB_TX);
	}
	
	/****** End of mailbox configuraion ******/

	//Enable interrupt on receive mailboxes
	CAN0->CAN_IER = can_ier;

	//Enable interrupt in NVIC 
	NVIC_EnableIRQ(ID_CAN0);

	//enable CAN
	CAN0->CAN_MR |= CAN_MR_CANEN;

	return 0;
}

/**
 * \brief Send can message from mailbox
 *
 * \param can_msg message to be sent
 *
 * \param tx_mb_id ID of transmit mailbox to be used
 *
 * \retval Success(0) or failure(1)
 */
uint8_t can_send(CAN_MESSAGE* can_msg, uint8_t tx_mb_id)
{
	//Check that mailbox is ready
	if(CAN0->CAN_MB[tx_mb_id].CAN_MSR & CAN_MSR_MRDY)
	{
		//Set message ID and use CAN 2.0B protocol
		CAN0->CAN_MB[tx_mb_id].CAN_MID = CAN_MID_MIDvA(can_msg->id) | CAN_MID_MIDE ;
		
		//Make sure message is not to long
		if(can_msg->data_length > 7){
			can_msg->data_length = 7;
			//Message is to long, sending only the first 8 bytes
		}
		//Put message in can data registers
		CAN0->CAN_MB[tx_mb_id].CAN_MDL = can_msg->data[3] << 24 | can_msg->data[2] << 16 | can_msg->data[1] << 8 | can_msg->data[0];
		CAN0->CAN_MB[tx_mb_id].CAN_MDH = can_msg->data[7] << 24 | can_msg->data[6] << 16 | can_msg->data[5] << 8 | can_msg->data[4];
		
		//Set message length and mailbox ready to send
		CAN0->CAN_MB[tx_mb_id].CAN_MCR = (can_msg->data_length << CAN_MCR_MDLC_Pos) | CAN_MCR_MTCR;
		return 0;
	}
	
	else //Mailbox busy
	{
		return 1;
	}
	
}

/**
 * \brief Read can message from mailbox
 *
 * \param can_msg struct instance to save received data
 *
 * \param rx_mb_id ID of receive mailbox to be used
 *
 * \retval Success(0) or failure(1)
 */
uint8_t can_receive(CAN_MESSAGE* can_msg, uint8_t rx_mb_id)
{
	//Check that mailbox is ready
	if(CAN0->CAN_MB[rx_mb_id].CAN_MSR & CAN_MSR_MRDY)
	{
		//Get data from CAN mailbox
		uint32_t data_low = CAN0->CAN_MB[rx_mb_id].CAN_MDL;
		uint32_t data_high = CAN0->CAN_MB[rx_mb_id].CAN_MDH;
		
		//Get message ID
		can_msg->id = (uint16_t)((CAN0->CAN_MB[rx_mb_id].CAN_MID & CAN_MID_MIDvA_Msk) >> CAN_MID_MIDvA_Pos);
		
		//Get data length
		can_msg->data_length = (uint8_t)((CAN0->CAN_MB[rx_mb_id].CAN_MSR & CAN_MSR_MDLC_Msk) >> CAN_MSR_MDLC_Pos);
		
		//Put data in CAN_MESSAGE object
		for(int i = 0; i < can_msg->data_length;i++)
		{
			if(i < 4)
			{
				can_msg->data[i] = (char)(data_low & 0xff);
				data_low = data_low >> 8;
			}
			else
			{
				can_msg->data[i] = (uint8_t)(data_high & 0xff);
				data_high = data_high >> 8;
			}
		}
		
		//Reset for new receive
		CAN0->CAN_MB[rx_mb_id].CAN_MMR = CAN_MMR_MOT_MB_RX;
		CAN0->CAN_MB[rx_mb_id].CAN_MCR |= CAN_MCR_MTCR;
		return 0;
	}
	else //Mailbox busy
	{
		return 1;
	}
}
#define SLIDER_NORMALIZER 255
void can_parse_frame(CAN_MESSAGE* can_msg) {
	
	switch(can_msg->id){
		case CAN_ID_MOTOR_TO_START:
			motor_enable();
			//printf("Received motor to start \n\r");
			motor_set_speed(-1000);
			break;
		case CAN_ID_CALIBRATE_ENCODER:
			encoder_calibrate();
			motor_controller_reset();
			break;
		case CAN_ID_JOYSTICK_STATE: {
			can_data_gamepad_state_t gamepad;
			memcpy(&gamepad, &can_msg->data, can_msg->data_length);
			uint16_t angle = 90+90*gamepad.stick_horizontal_pos/161;
			//printf("Horizontal pos: %d, angle: %u, slider pos: %u, button press: %u\n\r", gamepad.stick_horizontal_pos, angle, gamepad.slider_pos, gamepad.button_state);

			//float normalized_gamepad_slider_pos = (float)(gamepad.slider_pos/SLIDER_NORMALIZER);
			//printf("Noramlized slide pos %u \r\n", (uint16_t)(normalized_gamepad_slider_pos*100));
 			//motor_control_position(normalized_gamepad_slider_pos);
			motor_control_position(gamepad.slider_pos);
			pwm_set_angle(angle);
			if (gamepad.button_state == 1) {
				solenoid_disable();
			} else {
				solenoid_enable();
			}
			break;
		case CAN_ID_MOTOR_DISABLE:
			motor_disable();
			break;
		}
	}
}


