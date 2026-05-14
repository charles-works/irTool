#include "key_scan.h"
#include "board_config.h"
#include "clock.h"
#include "gpio.h"

void key_scan_init(void) {
    /* GPIO module owns electrical configuration. */
}

key_id_t key_scan_get_pressed(void) {
    if (gpio_read_active_low(KEY1_PORT, KEY1_PIN)) {
        return KEY_ID_1;
    }
    if (gpio_read_active_low(KEY2_PORT, KEY2_PIN)) {
        return KEY_ID_2;
    }
    if (gpio_read_active_low(KEY3_PORT, KEY3_PIN)) {
        return KEY_ID_3;
    }
    return KEY_ID_NONE;
}

key_id_t key_scan_wait_stable_press(void) {
    key_id_t first = key_scan_get_pressed();
    key_id_t second;
    if (first == KEY_ID_NONE) {
        return KEY_ID_NONE;
    }
    delay_ms(KEY_DEBOUNCE_MS);
    second = key_scan_get_pressed();
    return (first == second) ? first : KEY_ID_NONE;
}

bool_t key_scan_is_still_pressed(key_id_t key) {
    switch (key) {
    case KEY_ID_1: return gpio_read_active_low(KEY1_PORT, KEY1_PIN);
    case KEY_ID_2: return gpio_read_active_low(KEY2_PORT, KEY2_PIN);
    case KEY_ID_3: return gpio_read_active_low(KEY3_PORT, KEY3_PIN);
    default: return FALSE;
    }
}

void key_scan_wait_release(key_id_t key) {
    while (key_scan_is_still_pressed(key)) {
        delay_ms(5u);
    }
    delay_ms(KEY_RELEASE_DEBOUNCE_MS);
}

uint8_t key_scan_to_nec_command(key_id_t key) {
    switch (key) {
    case KEY_ID_1: return NEC_KEY1_CMD;
    case KEY_ID_2: return NEC_KEY2_CMD;
    case KEY_ID_3: return NEC_KEY3_CMD;
    default: return 0u;
    }
}
