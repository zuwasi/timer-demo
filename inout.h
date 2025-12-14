
#ifndef _input_h_
#define _input_h_

/* gets a digit from the input device */
int get_input_digit(void);

/* prints a string to the output device */
int print_string(char* str);

/* simple atoi for embedded (no stdlib) */
int simple_atoi(const char *str);

#endif /* _input_h_ */

