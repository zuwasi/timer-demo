/*
 * System initialization for STM32WB5x
 * Configures clocks and UART for serial I/O
 */

#include "stm32wb5x.h"

/* System clock frequency */
uint32_t SystemCoreClock = 4000000UL;

/*
 * Initialize system clocks and UART1 for serial communication
 * UART1: PA9 (TX), PA10 (RX) - 115200 baud
 */
void SystemInit(void)
{
    /* Enable GPIOA and USART1 clocks */
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    
    /* Small delay for clock to stabilize */
    volatile uint32_t delay = 100;
    while (delay--);
    
    /* Configure PA9 (TX) and PA10 (RX) as Alternate Function (AF7 = USART1) */
    /* MODER: 10 = Alternate function mode */
    GPIOA->MODER &= ~((3UL << (9 * 2)) | (3UL << (10 * 2)));
    GPIOA->MODER |= ((2UL << (9 * 2)) | (2UL << (10 * 2)));
    
    /* AFR: AF7 for USART1 */
    GPIOA->AFR[1] &= ~((0xFUL << ((9 - 8) * 4)) | (0xFUL << ((10 - 8) * 4)));
    GPIOA->AFR[1] |= ((7UL << ((9 - 8) * 4)) | (7UL << ((10 - 8) * 4)));
    
    /* Configure USART1: 115200 baud, 8N1 */
    /* BRR = fCK / baud = 4000000 / 115200 ≈ 35 */
    USART1->BRR = 35;
    
    /* Enable TX, RX, and USART */
    USART1->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

/*
 * Send a single character over UART1
 */
void uart_putchar(char c)
{
    /* Wait until TX buffer is empty */
    while (!(USART1->ISR & USART_ISR_TXE));
    USART1->TDR = (uint32_t)c;
}

/*
 * Receive a single character from UART1
 * Returns -1 if no character available (non-blocking check)
 */
int uart_getchar(void)
{
    if (USART1->ISR & USART_ISR_RXNE) {
        return (int)(USART1->RDR & 0xFF);
    }
    return -1;
}

/*
 * Blocking receive - waits for a character
 */
int uart_getchar_blocking(void)
{
    while (!(USART1->ISR & USART_ISR_RXNE));
    return (int)(USART1->RDR & 0xFF);
}

/*
 * Send a null-terminated string over UART1
 */
void uart_puts(const char *str)
{
    while (*str) {
        if (*str == '\n') {
            uart_putchar('\r');
        }
        uart_putchar(*str++);
    }
}
