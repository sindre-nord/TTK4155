// Implementation based on https://stackoverflow.com/a/827749
#include "circular_buffer.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define F_CPU 4915200 // Hz
#include <util/delay.h> // Uses F_CPU

void cb_allocate(volatile circular_buffer_t* cb, size_t capacity, size_t item_size)
{
    cb->buffer = malloc(capacity * item_size);
    // if(cb->buffer == NULL)
		// malloc failed
    cb->buffer_end = (unsigned char*)cb->buffer + capacity * item_size;
    cb->capacity = capacity;
    cb->count = 0;
    cb->item_size = item_size;
    cb->head = cb->buffer;
    cb->tail = cb->buffer;
}

void cb_free(volatile circular_buffer_t* cb)
{
    free(cb->buffer);
    // clear out other fields too, just to be safe
}

void cb_push_back(volatile circular_buffer_t* cb, const void* item)
{
    if(cb->count == cb->capacity) {
		// Queue is full. Overwrite oldest item
		cb->tail = (unsigned char*)cb->tail + cb->item_size;
		if(cb->tail == cb->buffer_end)
			cb->tail = cb->buffer;
		cb->count--;
	}
    memcpy(cb->head, item, cb->item_size);
    cb->head = (unsigned char*)cb->head + cb->item_size;
    if(cb->head == cb->buffer_end)
        cb->head = cb->buffer;
    cb->count++;
}

cb_pop_result_t cb_pop_front(volatile circular_buffer_t* cb, void* item)
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


void cb_test(void)
{
	volatile circular_buffer_t cb;
	const size_t capacity = 3;
	cb_allocate(&cb, capacity, sizeof(unsigned char));
	printf("capacity test: %i\n", cb.capacity == capacity);
	printf("count test: %i\n", cb.count == 0);
	printf("item size test: %i\n", cb.item_size == sizeof(unsigned char));
	char item = 'a';
	printf("pop empty test: %i\n", cb_pop_front(&cb, &item) == CB_QUEUE_EMPTY);
	_delay_ms(100);
	printf("item test: %i\n", item == 'a');
	_delay_ms(100);
	
	cb_push_back(&cb, &item);
	printf("count test 2: %i\n", cb.count == 1);
	item = 'b';
	cb_push_back(&cb, &item);
	printf("count test 3: %i\n", cb.count == 2);
	cb_push_back(&cb, &item);
	printf("count test 4: %i\n", cb.count == 3);
	cb_push_back(&cb, &item);
	printf("count test 5: %i\n", 3 == cb.count);
	_delay_ms(150);
	
	printf("pop test: %i\n", cb_pop_front(&cb, &item) == CB_POP_SUCCESSFUL);
	printf("count test 6: %i\n", cb.count == 2);
	printf("item test 2: %i\n", item == 'b');
	_delay_ms(150);
	item = 'a';
	printf("pop test: %i\n", cb_pop_front(&cb, &item) == CB_POP_SUCCESSFUL);
	printf("count test 7: %i\n", cb.count == 1);
	printf("item test 3: %i\n", item == 'b');
	_delay_ms(150);
	item = 'a';
	printf("pop test: %i\n", cb_pop_front(&cb, &item) == CB_POP_SUCCESSFUL);
	printf("count test 8: %i\n", cb.count == 0);
	printf("item test 4: %i\n", item == 'b');
	_delay_ms(150);
	item = 'c';
	printf("pop empty test: %i\n", cb_pop_front(&cb, &item) == CB_QUEUE_EMPTY);
	printf("count test 9: %i\n", cb.count == 0);
	printf("item test 5: %i\n", item == 'c');
	_delay_ms(150);
	
	cb_free(&cb);
	return;
}