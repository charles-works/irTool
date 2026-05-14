#include "gpio.h"
#include "board_config.h"

static uint8_t pin_mask(uint8_t pin) {
    return (uint8_t)(1u << pin);
}

static void config_input_pullup(GPIO_Port_TypeDef *port, uint8_t pin, bool_t irq_enable) {
    uint8_t mask = pin_mask(pin);
    port->DDR &= (uint8_t)~mask;
    port->CR1 |= mask;
    if (irq_enable) {
        port->CR2 |= mask;
    } else {
        port->CR2 &= (uint8_t)~mask;
    }
}

void gpio_ir_idle(void) {
    uint8_t mask = pin_mask(IR_PIN);
    IR_PORT->ODR &= (uint8_t)~mask;
    IR_PORT->DDR |= mask;
    IR_PORT->CR1 |= mask;
    IR_PORT->CR2 &= (uint8_t)~mask;
}

void gpio_ir_enable(void) {
    gpio_ir_idle();
}

void gpio_ir_set(bool_t on) {
    uint8_t mask = pin_mask(IR_PIN);
    if (on) {
        IR_PORT->ODR |= mask;
    } else {
        IR_PORT->ODR &= (uint8_t)~mask;
    }
}

bool_t gpio_read_active_low(GPIO_Port_TypeDef *port, uint8_t pin) {
    return ((port->IDR & pin_mask(pin)) == 0u) ? TRUE : FALSE;
}

void gpio_init(void) {
    gpio_ir_idle();
    config_input_pullup(KEY1_PORT, KEY1_PIN, TRUE);
    config_input_pullup(KEY2_PORT, KEY2_PIN, TRUE);
    config_input_pullup(KEY3_PORT, KEY3_PIN, TRUE);
    config_input_pullup(DBG_STRAP_PORT, DBG_STRAP_PIN, FALSE);

    /* Falling-edge wake intent for active-low keys; verify final EXTI grouping
     * on hardware before claiming low-power current or wake timing.
     */
    EXTI_CR1 = 0xAAu;
    EXTI_CR2 = 0x0Au;
}
