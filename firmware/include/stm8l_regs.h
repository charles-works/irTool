#ifndef STM8L_REGS_H
#define STM8L_REGS_H

/* Minimal STM8L register surface for the frameworkless PlatformIO build.
 * Target default: STM8L051F3P6.  Add only registers used by this firmware.
 */
typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long  uint32_t;

typedef unsigned char bool_t;
#define TRUE  ((bool_t)1u)
#define FALSE ((bool_t)0u)

#define __IO volatile
#define NOP()       __asm__("nop")
#define HALT_CPU()  __asm__("halt")
#define RIM()       __asm__("rim")
#define SIM()       __asm__("sim")

typedef struct {
    __IO uint8_t ODR;
    __IO uint8_t IDR;
    __IO uint8_t DDR;
    __IO uint8_t CR1;
    __IO uint8_t CR2;
} GPIO_Port_TypeDef;

#define GPIOA ((GPIO_Port_TypeDef *)0x5000)
#define GPIOB ((GPIO_Port_TypeDef *)0x5005)
#define GPIOC ((GPIO_Port_TypeDef *)0x500A)
#define GPIOD ((GPIO_Port_TypeDef *)0x500F)
#define GPIOE ((GPIO_Port_TypeDef *)0x5014)
#define GPIOF ((GPIO_Port_TypeDef *)0x5019)

#define CLK_ICKCR   (*(volatile uint8_t *)0x50C0)
#define CLK_ECKCR   (*(volatile uint8_t *)0x50C1)
#define CLK_CKDIVR  (*(volatile uint8_t *)0x50C6)
#define CLK_PCKENR1 (*(volatile uint8_t *)0x50C7)
#define CLK_PCKENR2 (*(volatile uint8_t *)0x50CA)

#define EXTI_CR1    (*(volatile uint8_t *)0x50A0)
#define EXTI_CR2    (*(volatile uint8_t *)0x50A1)

#define PWR_CSR1    (*(volatile uint8_t *)0x50B2)
#define PWR_CSR2    (*(volatile uint8_t *)0x50B3)

#ifndef NULL
#define NULL ((void *)0)
#endif

#endif
