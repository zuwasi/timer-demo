
/* 
 * Implements routines from inout.h for stdin/stdout
 */

#include "inout.h"
#include "consts.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Grabs input from stdin
 */
int get_input_digit()
{
    char c;
    int i;
    char buf[BUF_SIZE];

    fflush(stdout);
    for (i = 0; i < BUF_SIZE - 1; i++) {
        c = getchar();
        if (c == '\n' || c == EOF) {
            break;
        } else if (isdigit(c)) {
            buf[i] = c;
        }
    }
    
    buf[i] = '\0';
    return (atoi(buf));
}

/*
 * Print string to stdout
 */
int print_string(char* str)
{
    return (printf(str));
}
