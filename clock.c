
/*
 * Clock related functions
 */
#include <stdio.h>
#include "clock.h"
#include "consts.h"
#include "inout.h"

/*
 * Print the current time
 */
void display_time()
{
    char buf[BUF_SIZE];
    time_t the_time = time(NULL);
    char *t = ctime(&the_time);
    
    sprintf(buf, "\n\nCurrent Time and Date is %s\n\n", t);
    print_string(buf);
}

/*
 * BUG #4: UNINITIALIZED VARIABLE USE
 * The variable 'offset' is only initialized on one path.
 * On the else path, it remains uninitialized but is used.
 * Flow analysis tracks initialization state across branches.
 */
int calculate_time_offset(int hours, int use_dst)
{
    int offset;  /* Not initialized! */
    int result;
    
    if (use_dst) {
        offset = 3600;  /* DST offset in seconds */
    }
    /* BUG: offset not initialized on else path! */
    
    result = hours * 3600 + offset;  /* Use of potentially uninitialized 'offset' */
    return result;
}

/*
 * BUG #10: RESOURCE LEAK - FILE NOT CLOSED
 * File is opened but not closed on one execution path.
 * Flow analysis tracks resource state across all paths.
 */
void log_time_to_file(const char* filename)
{
    FILE* fp;
    time_t now;
    char* time_str;
    
    fp = fopen(filename, "a");
    if (fp == NULL) {
        return;  /* OK - file not opened */
    }
    
    now = time(NULL);
    time_str = ctime(&now);
    
    if (time_str == NULL) {
        return;  /* BUG: fp is open but not closed! Resource leak! */
    }
    
    fprintf(fp, "Time: %s", time_str);
    fclose(fp);  /* Only closed on success path */
}

/* 
 * Dummy Function -- time always taken from system
 */
void set_time(time_t new_time)
{
}

