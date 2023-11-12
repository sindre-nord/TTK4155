/*
 * ir.h
 *
 * Created: 23.10.2023 11:44:52
 *  Author: jknewall
 */ 


#ifndef IR_H_
#define IR_H_
#include <stdint.h>

void ir_init(void);

uint16_t ir_read_value(void);


#endif /* IR_H_ */