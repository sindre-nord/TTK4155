#include "circular_buffer.h"

#include <string.h>
#include <stdlib.h>

void cb_allocate(circular_buffer_t* cb, size_t capacity, size_t item_size)
{
    cb->buffer = malloc(capacity * item_size);
    if(cb->buffer == NULL)
        // handle error
    cb->buffer_end = (unsigned char*)cb->buffer + capacity * item_size;
    cb->capacity = capacity;
    cb->count = 0;
    cb->item_size = item_size;
    cb->head = cb->buffer;
    cb->tail = cb->buffer;
}

void cb_free(circular_buffer_t* cb)
{
    free(cb->buffer);
    // clear out other fields too, just to be safe
}

void cb_push_back(circular_buffer_t* cb, const void* item)
{
    if(cb->count == cb->capacity) {
		// Queue is full. Oldest item will be overwritten below
		cb->count--;
	}
    memcpy(cb->head, item, cb->item_size);
    cb->head = (unsigned char*)cb->head + cb->item_size;
    if(cb->head == cb->buffer_end)
        cb->head = cb->buffer;
    cb->count++;
}

cb_pop_result_t cb_pop_front(circular_buffer_t* cb, void* item)
{
    if(cb->count == 0){
        return CB_QUEUE_EMPTY;
    }
    memcpy(item, cb->tail, cb->item_size);
    cb->tail = (unsigned char*)cb->tail + cb->item_size;
    if(cb->tail == cb->buffer_end)
        cb->tail = cb->buffer;
    cb->count--;
    return CB_POP_SUCCESSFUL;
}
