/*
 * can.h
 *
 * Created: 29.09.2023 15:09:04
 *  Author: jknewall
 */ 


#ifndef CAN_H_
#define CAN_H_

#include <stddef.h>
#include <stdint.h>

typedef struct {
	uint16_t id; // Standard id: 11 bit length
	uint8_t data_length;
	unsigned char data[8];
} can_frame_t;

void can_init(void);
void can_send(can_frame_t* frame);
size_t can_receive(can_frame_t frame[], size_t max_frames);
void can_receive_blocking(can_frame_t* frame);

#endif /* CAN_H_ */