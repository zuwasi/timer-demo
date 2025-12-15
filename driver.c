
/*
 * driver class to test the timer.
 */

#include <stdio.h>
#include <string.h>
#include "clock.h"
#include "consts.h"
#include "inout.h"
#include "timer.h"

/*
 * BUG #8: RETURN OF STACK ADDRESS
 * Returns pointer to local array which goes out of scope.
 * Flow analysis detects escaping stack addresses.
 */
char* get_formatted_menu_title(int menu_id)
{
    char local_buffer[50];  /* Local stack variable */
    
    sprintf(local_buffer, "=== Menu #%d ===", menu_id);
    
    return local_buffer;  /* BUG: Returning address of local variable! */
}

/*
 * BUG #5: BUFFER OVERFLOW
 * User input is used to control copy length without bounds checking.
 * Flow analysis tracks tainted data into dangerous operations.
 */
void copy_user_input(char* dest, int dest_size)
{
    char temp[10];  /* Small buffer */
    int copy_len;
    
    print_string("Enter number of chars to copy: ");
    copy_len = get_input_digit();  /* Tainted data from user */
    
    /* BUG: copy_len from user input used without validation */
    /* If user enters > 10, this overflows temp buffer */
    memset(temp, 'A', copy_len);  /* Buffer overflow if copy_len > 10! */
    temp[copy_len] = '\0';
    
    if (dest && dest_size > 0) {
        strncpy(dest, temp, dest_size - 1);
        dest[dest_size - 1] = '\0';
    }
}

int print_menu_get_action()
{
    print_string("**************************************************\n");
    print_string("*                  Timer Menu                    *\n");
    print_string("* -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ *\n");
    print_string("*                                                *\n");
    print_string("* 1) Add a timer                                 *\n");
    print_string("* 2) Remove a timer                              *\n");
    print_string("* 3) List all timers                             *\n");
    print_string("* 4) Show time                                   *\n");
    print_string("*                                                *\n");
    print_string("* 9) Exit                                        *\n");
    print_string("*                                                *\n");
    print_string("**************************************************\n\n");
    
    print_string("Enter selection, when done press <enter> > ");
    return (get_input_digit());
}

void main_loop()
{
    while (1) {
        int res, i = print_menu_get_action();
        
        switch(i)
        {
        case 1:
            res = add_timer();
            if (ERROR_CODE == res) {
                print_string("\nError ... timer not added!\n");
            } else {
                print_string("\nTimer added!\n");
            }
            break;
        case 2:
            print_string("Which timer should I nuke? > ");
            delete_timer_record(get_input_digit());
            break;
        case 3:
            list_timers();
            break;
        case 4:
            display_time();
            break;
        case 9:
            /* Exit */
            print_string("\nGoodbye\n\n");
            return;
        default:
            /* do nothing ... reprint menu */
            print_string("\nInvalid Action ... try again!\n");
            break;
        }
    }
}

int main()
{
    init_timer();     /* setup */
    main_loop();      /* loop until user quits */
    uninit_timer();   /* tear down */
    return 0;
}

