#ifndef KEY_SCAN_H
#define KEY_SCAN_H

#include "stm8l_regs.h"

typedef enum {
    KEY_ID_NONE = 0,
    KEY_ID_1,
    KEY_ID_2,
    KEY_ID_3
} key_id_t;

void key_scan_init(void);
key_id_t key_scan_get_pressed(void);
key_id_t key_scan_wait_stable_press(void);
bool_t key_scan_is_still_pressed(key_id_t key);
void key_scan_wait_release(key_id_t key);
uint8_t key_scan_to_nec_command(key_id_t key);

#endif
