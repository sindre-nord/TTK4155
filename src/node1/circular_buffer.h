// Implementation based on https://stackoverflow.com/a/827749

#pragma once
#include <stddef.h>


typedef struct
{
    void* buffer;     // data buffer
    void* buffer_end; // end of data buffer
    size_t capacity;  // maximum number of items in the buffer
    size_t count;     // number of items in the buffer
    size_t item_size;        // size of each item in the buffer
    void* head;       // pointer to head
    void* tail;       // pointer to tail
} circular_buffer_t;

void cb_allocate(volatile circular_buffer_t* cb, size_t capacity, size_t item_size);
void cb_free(volatile circular_buffer_t* cb);
void cb_push_back(volatile circular_buffer_t* cb, const void* item);

typedef enum {
    CB_POP_SUCCESSFUL,
    CB_QUEUE_EMPTY
} cb_pop_result_t;
cb_pop_result_t cb_pop_front(volatile circular_buffer_t* cb, void* item);

void cb_test(void);