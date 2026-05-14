#include "clock.h"
#include "board_config.h"

void clock_init(void) {
    /* HSI on, no prescaler: 16 MHz core clock for deterministic NEC timing. */
    CLK_ICKCR |= 0x01u;      /* HSIEN */
    while ((CLK_ICKCR & 0x02u) == 0u) {
        /* wait HSIRDY */
    }
    CLK_CKDIVR = 0x00u;
}

void delay_us(uint16_t us) {
    while (us-- != 0u) {
        NOP(); NOP(); NOP(); NOP();
        NOP(); NOP(); NOP(); NOP();
        NOP(); NOP(); NOP(); NOP();
    }
}

void delay_ms(uint16_t ms) {
    while (ms-- != 0u) {
        delay_us(1000u);
    }
}
