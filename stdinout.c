
/* 
 * Implements routines from inout.h for stdin/stdout
 */

#include "inout.h"
#include "consts.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Grabs input from stdin
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
 * IMPACT:
 * - Prevents infinite loops when input ends (Ctrl+Z on Windows, Ctrl+D on Linux)
 * - Prevents infinite loops when using piped/redirected input
 * - Correctly distinguishes EOF from valid byte 0xFF
 */
int get_input_digit()
{
    /*
     * FIXED: Changed from 'char c' to 'int c'
     * getchar() returns int to accommodate EOF (-1) plus all valid char values (0-255)
     */
    int c;
    int i;
    char buf[BUF_SIZE];

    fflush(stdout);
    for (i = 0; i < BUF_SIZE - 1; i++) {
        c = getchar();
        /*
         * Now EOF comparison works correctly because 'c' is an int.
         * If getchar() returns EOF (-1), this comparison will be true.
         */
        if (c == '\n' || c == EOF) {
            break;
        } else if (isdigit(c)) {
            /*
             * Cast to char only when storing - at this point we know
             * c is a valid digit character (0-9), not EOF
             */
            buf[i] = (char)c;
        }
    }
    
    buf[i] = '\0';
    return atoi(buf);
}

/*
 * Print string to stdout
 */
int print_string(char* str)
{
    return (printf(str));
}

/*
 * BUG #6: DIVISION BY ZERO
 * Divisor comes from user input and may be zero.
 * Flow analysis tracks tainted data to division operations.
 */
int calculate_average(int total)
{
    int count;
    int average;
    
    print_string("Enter count for average: ");
    count = get_input_digit();  /* Tainted data from user */
    
    /* BUG: No check if count is zero before division */
    average = total / count;  /* Division by zero if count == 0! */
    
    return average;
}

/*
 * BUG #1: NULL POINTER DEREFERENCE ON CONDITIONAL PATH  
 * Pointer is dereferenced before null check on one path.
 * Flow analysis tracks null state across complex conditionals.
 */
int process_input_buffer(char* buffer, int mode)
{
    int result = 0;
    int len;
    
    if (mode == 1) {
        /* Dereference before null check */
        len = strlen(buffer);  /* BUG: buffer might be NULL! */
        result = len * 2;
    }
    
    if (buffer == NULL) {
        return -1;  /* Too late - already dereferenced above! */
    }
    
    if (mode == 2) {
        result = buffer[0];  /* Safe here due to null check above */
    }
    
    return result;
}
