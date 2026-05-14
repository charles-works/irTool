#ifndef IR_NEC_H
#define IR_NEC_H

#include "stm8l_regs.h"

void ir_nec_init(void);
void ir_nec_send_frame(uint8_t command);
void ir_nec_send_repeat(void);

#endif
