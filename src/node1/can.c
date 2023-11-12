/*
 * can.c
 *
 * Created: 29.09.2023 15:08:37
 *  Author: jknewall
 */ 
#include "can.h"

#include <stdio.h>

#include "mcp2515.h"
#include <avr/io.h>

void can_init(void) {
	mcp_init();
}

void can_send(can_frame_t* frame) {
	uint8_t available_buffer = mcp_wait_for_empty_tx_buffers();
	mcp_write_standard_id_field(available_buffer, frame->id);
	mcp_write_data_field(available_buffer, frame->data, frame->data_length);
	mcp_request_to_send(available_buffer);
}

size_t can_receive(can_frame_t frame[], size_t max_frames) {
	int8_t filled_buffer_index = mcp_check_for_full_rx_buffer();
	if (filled_buffer_index == -1) {
		return 0;
	}
	mcp_read_standard_id_field(filled_buffer_index, &frame->id);
	frame->data_length = mcp_read_data_field(filled_buffer_index, frame->data);
	mcp_signal_available_rx_buffer(filled_buffer_index);
	return 1;
}

void can_receive_blocking(can_frame_t* frame) {
	uint8_t filled_buffer_index = mcp_wait_for_full_rx_buffers();
	mcp_read_standard_id_field(filled_buffer_index, &frame->id);
	frame->data_length = mcp_read_data_field(filled_buffer_index, frame->data);
	mcp_signal_available_rx_buffer(filled_buffer_index);
}


