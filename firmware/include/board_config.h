#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

#include "stm8l_regs.h"

/* Active hardware target: STM8L051F3P6, TSSOP-20.
 * Supply class: 1.8 V..3.6 V, suitable for CR2032 operation after pulse-load
 * validation. Debug access: preserve SWIM, NRST, VDD and GND test points.
 */
#define F_CPU_HZ 16000000UL

/* First-pass debug-friendly pin map for STM8L051F3P6.
 * PA0/SWIM/BEEP/IR_TIM is kept as the IR output candidate because it has the
 * IR-related alternate function; NRST remains reserved for reset/debug.
 * Keys are ordinary active-low GPIO inputs with pull-ups and EXTI wake intent.
 * Verify exact package pin numbers against the datasheet during schematic
 * capture/bring-up before PCB layout.
 */
#define IR_PORT GPIOA
#define IR_PIN  0u

#define KEY1_PORT GPIOB
#define KEY1_PIN  0u
#define KEY2_PORT GPIOB
#define KEY2_PIN  1u
#define KEY3_PORT GPIOC
#define KEY3_PIN  3u

#define DBG_STRAP_PORT GPIOC
#define DBG_STRAP_PIN  4u

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
