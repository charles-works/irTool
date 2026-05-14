#ifndef CLOCK_H
#define CLOCK_H

#include "stm8l_regs.h"

void clock_init(void);
void delay_us(uint16_t us);
void delay_ms(uint16_t ms);

#endif
