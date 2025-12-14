/*
 * STM32WB5x Register Definitions (Minimal for UART I/O)
 * Based on STM32WB55 Reference Manual
 */

#ifndef STM32WB5X_H
#define STM32WB5X_H

#include <stdint.h>

/* Base addresses */
#define PERIPH_BASE           0x40000000UL
#define APB1PERIPH_BASE       PERIPH_BASE
#define APB2PERIPH_BASE       (PERIPH_BASE + 0x00010000UL)
#define AHB1PERIPH_BASE       (PERIPH_BASE + 0x00020000UL)
#define AHB2PERIPH_BASE       (PERIPH_BASE + 0x08000000UL)

/* RCC */
#define RCC_BASE              (AHB1PERIPH_BASE + 0x00001000UL)

/* GPIO */
#define GPIOA_BASE            (AHB2PERIPH_BASE + 0x00000000UL)
#define GPIOB_BASE            (AHB2PERIPH_BASE + 0x00000400UL)

/* USART1 */
#define USART1_BASE           (APB2PERIPH_BASE + 0x00003800UL)

/* RCC Register Structure */
typedef struct {
    volatile uint32_t CR;
    volatile uint32_t ICSCR;
    volatile uint32_t CFGR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t PLLSAI1CFGR;
    volatile uint32_t RESERVED0;
    volatile uint32_t CIER;
    volatile uint32_t CIFR;
    volatile uint32_t CICR;
    volatile uint32_t SMPSCR;
    volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    volatile uint32_t AHB3RSTR;
    volatile uint32_t RESERVED1;
    volatile uint32_t APB1RSTR1;
    volatile uint32_t APB1RSTR2;
    volatile uint32_t APB2RSTR;
    volatile uint32_t APB3RSTR;
    volatile uint32_t AHB1ENR;
    volatile uint32_t AHB2ENR;
    volatile uint32_t AHB3ENR;
    volatile uint32_t RESERVED2;
    volatile uint32_t APB1ENR1;
    volatile uint32_t APB1ENR2;
    volatile uint32_t APB2ENR;
} RCC_TypeDef;

/* GPIO Register Structure */
typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFR[2];
    volatile uint32_t BRR;
} GPIO_TypeDef;

/* USART Register Structure */
typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t BRR;
    volatile uint32_t GTPR;
    volatile uint32_t RTOR;
    volatile uint32_t RQR;
    volatile uint32_t ISR;
    volatile uint32_t ICR;
    volatile uint32_t RDR;
    volatile uint32_t TDR;
    volatile uint32_t PRESC;
} USART_TypeDef;

/* Peripheral declarations */
#define RCC                   ((RCC_TypeDef *)RCC_BASE)
#define GPIOA                 ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB                 ((GPIO_TypeDef *)GPIOB_BASE)
#define USART1                ((USART_TypeDef *)USART1_BASE)

/* RCC bit definitions */
#define RCC_AHB2ENR_GPIOAEN   (1UL << 0)
#define RCC_AHB2ENR_GPIOBEN   (1UL << 1)
#define RCC_APB2ENR_USART1EN  (1UL << 14)

/* USART bit definitions */
#define USART_CR1_UE          (1UL << 0)
#define USART_CR1_RE          (1UL << 2)
#define USART_CR1_TE          (1UL << 3)
#define USART_ISR_TXE         (1UL << 7)
#define USART_ISR_RXNE        (1UL << 5)
#define USART_ISR_TC          (1UL << 6)

/* System clock frequency (default MSI at 4MHz) */
extern uint32_t SystemCoreClock;

#endif /* STM32WB5X_H */
