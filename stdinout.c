/* 
 * Implements routines from inout.h for STM32WB5x UART I/O
 * 
 * EMBEDDED VERSION:
 * This file replaces stdin/stdout with UART1 communication
 * for the STM32WB55 microcontroller.
 */

#include "inout.h"
#include "consts.h"

#include <ctype.h>

/* 
 * External UART functions from system_stm32wb5x.c
 */
extern void uart_putchar(char c);
extern void uart_puts(const char *str);
extern int uart_getchar_blocking(void);

/*
 * Grabs input from UART
 * 
 * FIX: BD-PB-EOFCOMP - EOF comparison issue
 * =========================================
 * 
 * PROBLEM:
 * The original code declared 'c' as 'char' type:
 *     char c;
 *     c = getchar();
 *     if (c == EOF) { ... }
 * 
 * This is problematic because:
 * 1. getchar() returns an 'int', not a 'char'
 * 2. EOF is typically defined as -1 (an int value)
 * 3. When getchar()'s int return value is stored in a char:
 *    - On systems where 'char' is UNSIGNED (0-255): 
 *      EOF (-1) becomes 255, so (c == EOF) is ALWAYS FALSE
 *      This causes an INFINITE LOOP when EOF is reached
 *    - On systems where 'char' is SIGNED (-128 to 127):
 *      The valid character 0xFF (255) becomes -1, 
 *      making it indistinguishable from EOF
 * 
 * SOLUTION:
 * Declare 'c' as 'int' to preserve the full return value from getchar().
 * This allows proper detection of EOF (-1) vs valid characters (0-255).
 * Only cast to 'char' when storing in the buffer after validation.
 * 
 * EMBEDDED NOTE:
 * On embedded systems, EOF is less common but the same principle applies
 * for detecting special control characters or connection loss.
 */
int get_input_digit()
{
    /*
     * FIXED: Changed from 'char c' to 'int c'
     * uart_getchar_blocking() returns int to accommodate all valid char values
     */
    int c;
    int i;
    char buf[BUF_SIZE];

    for (i = 0; i < BUF_SIZE - 1; i++) {
        c = uart_getchar_blocking();
        
        /* Echo character back to terminal */
        if (c >= 32 && c < 127) {
            uart_putchar((char)c);
        }
        
        /*
         * Now comparison works correctly because 'c' is an int.
         */
        if (c == '\n' || c == '\r' || c < 0) {
            uart_puts("\r\n");
            break;
        } else if (isdigit(c)) {
            /*
             * Cast to char only when storing - at this point we know
             * c is a valid digit character (0-9)
             */
            buf[i] = (char)c;
        }
    }
    
    buf[i] = '\0';
    return simple_atoi(buf);
}

/*
 * Simple atoi implementation for embedded (no stdlib dependency)
 */
int simple_atoi(const char *str)
{
    int result = 0;
    int sign = 1;
    
    /* Skip whitespace */
    while (*str == ' ' || *str == '\t') {
        str++;
    }
    
    /* Handle sign */
    if (*str == '-') {
        sign = -1;
        str++;
    } else if (*str == '+') {
        str++;
    }
    
    /* Convert digits */
    while (*str >= '0' && *str <= '9') {
        result = result * 10 + (*str - '0');
        str++;
    }
    
    return sign * result;
}

/*
 * Print string to UART
 */
int print_string(char* str)
{
    int count = 0;
    while (*str) {
        if (*str == '\n') {
            uart_putchar('\r');
            count++;
        }
        uart_putchar(*str++);
        count++;
    }
    return count;
}
