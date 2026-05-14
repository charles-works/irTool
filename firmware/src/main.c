#include "board_config.h"
#include "clock.h"
#include "gpio.h"
#include "ir_nec.h"
#include "key_scan.h"
#include "power.h"

static void optional_swim_attach_window(void) {
#if ENABLE_SWIM_DEBUG_WINDOW
    delay_ms(SWIM_DEBUG_WINDOW_MS);
#endif
}

static void transmit_key_until_release(key_id_t key) {
    uint8_t command = key_scan_to_nec_command(key);
    if (command == 0u) {
        return;
    }

    ir_nec_send_frame(command);
    while (key_scan_is_still_pressed(key)) {
        delay_ms(NEC_REPEAT_PERIOD_MS);
        if (key_scan_is_still_pressed(key)) {
            ir_nec_send_repeat();
        }
    }
    key_scan_wait_release(key);
}

void main(void) {
    key_id_t key;

    clock_init();
    gpio_init();
    key_scan_init();
    power_init();

    optional_swim_attach_window();
    ir_nec_init();

    while (1) {
        key = key_scan_wait_stable_press();
        if (key != KEY_ID_NONE) {
            transmit_key_until_release(key);
        }
        power_enter_halt_until_key();
    }
}
