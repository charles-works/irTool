#include "ir_nec.h"
#include "board_config.h"
#include "clock.h"
#include "gpio.h"

#define NEC_CARRIER_HALF_PERIOD_US 13u
#define NEC_LEADER_MARK_US        9000u
#define NEC_LEADER_SPACE_US       4500u
#define NEC_REPEAT_SPACE_US       2250u
#define NEC_BIT_MARK_US            560u
#define NEC_ZERO_SPACE_US          560u
#define NEC_ONE_SPACE_US          1690u

static void carrier_mark(uint16_t duration_us) {
    uint16_t cycles = (uint16_t)(duration_us / (NEC_CARRIER_HALF_PERIOD_US * 2u));
    while (cycles-- != 0u) {
        gpio_ir_set(TRUE);
        delay_us(NEC_CARRIER_HALF_PERIOD_US);
        gpio_ir_set(FALSE);
        delay_us(NEC_CARRIER_HALF_PERIOD_US);
    }
    gpio_ir_set(FALSE);
}

static void space(uint16_t duration_us) {
    gpio_ir_set(FALSE);
    delay_us(duration_us);
}

static void send_byte_lsb(uint8_t data) {
    uint8_t i;
    for (i = 0u; i < 8u; i++) {
        carrier_mark(NEC_BIT_MARK_US);
        space((data & 0x01u) != 0u ? NEC_ONE_SPACE_US : NEC_ZERO_SPACE_US);
        data >>= 1;
    }
}

void ir_nec_init(void) {
    gpio_ir_enable();
}

void ir_nec_send_frame(uint8_t command) {
    uint8_t inverse = (uint8_t)~command;

    carrier_mark(NEC_LEADER_MARK_US);
    space(NEC_LEADER_SPACE_US);
    send_byte_lsb(NEC_ADDR);      /* 0x80 */
    send_byte_lsb(NEC_ADDR_INV);  /* 0x7F */
    send_byte_lsb(command);       /* KEY1 0xC0, KEY2 0x40, KEY3 0x60 */
    send_byte_lsb(inverse);       /* 0x3F, 0xBF, 0x9F */
    carrier_mark(NEC_BIT_MARK_US);
    space(1000u);
}

void ir_nec_send_repeat(void) {
    carrier_mark(NEC_LEADER_MARK_US);
    space(NEC_REPEAT_SPACE_US);
    carrier_mark(NEC_BIT_MARK_US);
    space(1000u);
}
