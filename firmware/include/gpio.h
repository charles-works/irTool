#ifndef GPIO_H
#define GPIO_H

#include "stm8l_regs.h"

void gpio_init(void);
void gpio_ir_enable(void);
void gpio_ir_idle(void);
void gpio_ir_set(bool_t on);
bool_t gpio_read_active_low(GPIO_Port_TypeDef *port, uint8_t pin);

#endif
