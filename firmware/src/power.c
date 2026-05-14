#include "power.h"
#include "gpio.h"
#include "stm8l_regs.h"

void power_init(void) {
    /* Compile-first conservative setup; actual halt current is not bench-verified. */
    PWR_CSR1 = PWR_CSR1;
    PWR_CSR2 = PWR_CSR2;
}

void power_prepare_sleep(void) {
    gpio_ir_set(FALSE);
}

void power_enter_halt_until_key(void) {
    power_prepare_sleep();
    RIM();
    HALT_CPU();
}
