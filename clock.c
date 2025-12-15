
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
 * FIX: 15-Dec-2025 Daniel Liezrowice
 * Issue: BD-PB-NOTINIT - Variable 'offset' was only initialized on one branch (use_dst=true).
 * On the else path, offset remained uninitialized causing undefined behavior.
 * Resolution: Initialize offset to 0 at declaration to ensure it has a valid value on all paths.
 */
int calculate_time_offset(int hours, int use_dst)
{
    int offset = 0;
    int result;
    
    if (use_dst) {
        offset = 3600;
    }
    
    result = hours * 3600 + offset;
    return result;
}

/*
 * FIX: 15-Dec-2025 Daniel Liezrowice
 * Issue: BD-RES-LEAKS - File handle 'fp' was not closed on the error path when time_str was NULL.
 * This caused a resource leak as the file remained open.
 * Resolution: Added fclose(fp) before the early return when time_str is NULL.
 */
void log_time_to_file(const char* filename)
{
    FILE* fp;
    time_t now;
    char* time_str;
    
    fp = fopen(filename, "a");
    if (fp == NULL) {
        return;
    }
    
    now = time(NULL);
    time_str = ctime(&now);
    
    if (time_str == NULL) {
        fclose(fp);
        return;
    }
    
    fprintf(fp, "Time: %s", time_str);
    fclose(fp);
}

/* 
 * Dummy Function -- time always taken from system
 */
void set_time(time_t new_time)
{
}

