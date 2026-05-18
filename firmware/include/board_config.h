#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

#include "stm8l_regs.h"

/* Active hardware target: STM8L051F3P6, TSSOP-20.
 * Supply class: 1.8 V..3.6 V, suitable for CR2032 operation after pulse-load
 * validation. Debug access: preserve SWIM, NRST, VDD and GND test points.
 */
#define F_CPU_HZ 16000000UL

/* First-pass debug-friendly pin map for STM8L051F3P6 TSSOP-20.
 * Legal package facts used by this revision:
 *   - PA0 is pin 3 and doubles as SWIM/BEEP/IR_TIM.
 *   - NRST/PA1 is pin 4 and remains the reset/debug access point.
 *   - PB0/PB1 are pins 10/11 and are convenient for key inputs.
 *   - PD0 is pin 9 and is a valid third key input if pull-up/debounce are used.
 * Keys are active-low GPIO inputs with internal pull-ups and EXTI wake intent.
 */
#define IR_PORT GPIOA
#define IR_PIN  0u

#define KEY1_PORT GPIOB
#define KEY1_PIN  0u
#define KEY2_PORT GPIOB
#define KEY2_PIN  1u
#define KEY3_PORT GPIOD
#define KEY3_PIN  0u

#define DBG_STRAP_PORT GPIOC
#define DBG_STRAP_PIN  1u

#define NEC_ADDR       0x80u
#define NEC_ADDR_INV   0x7Fu
#define NEC_KEY1_CMD   0xC0u
#define NEC_KEY2_CMD   0x40u
#define NEC_KEY3_CMD   0x60u

#define KEY_DEBOUNCE_MS          20u
#define KEY_RELEASE_DEBOUNCE_MS  20u
#define NEC_REPEAT_PERIOD_MS     108u

/* Early firmware bring-up keeps a PA0/SWIM attach window before using IR. */
#define ENABLE_SWIM_DEBUG_WINDOW 1u
#define SWIM_DEBUG_WINDOW_MS     1000u

#endif
