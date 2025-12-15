
/*
 * Timer class, stores timer records-
 * Modified By Daniel Liezrowice 12/12/2025
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clock.h"
#include "consts.h"
#include "inout.h"
#include "timer.h"


static struct timer_record* timer_records[BUF_SIZE];
const int max_records = BUF_SIZE;
static int curr_index = 0;
static struct timer_record* cached_record = NULL;  /* BUG #2: Used for use-after-free demo */

void init_timer()
{
    memset(timer_records, 0, sizeof(struct timer_record*) * BUF_SIZE); 
}

void uninit_timer()
{
    /* Remove all timer records */
    int i;

    for (i = 0; i < max_records; i++) {
        delete_timer_record(i);
    }
    
    /* BUG #2: USE-AFTER-FREE
     * cached_record points to freed memory after delete_timer_record frees it.
     * Accessing cached_record->channel reads freed memory.
     * Flow analysis tracks that cached_record was freed indirectly.
     */
    if (cached_record != NULL) {
        print_string("Last cached channel was: ");
        printf("%d\n", cached_record->channel);  /* Use after free! */
    }
}

/*
 * Queries user for timer information
 */
struct timer_record* query_user()
{
    /*
     * FIX: 15-Dec-2025 Daniel Liezrowice
     * Issue: BD-PB-VOVR - Variables start_m, end_h, end_m were declared but never used.
     * Parasoft Flow Analysis detected these as unused values on any reachable path.
     * Resolution: Removed unused variables to eliminate dead code warning.
     * Only start_h is retained as it is actually used in the function.
     */
    int start_h;
    struct timer_record* the_record;
    time_t timer;
    struct tm* tm_tmp;

    timer = time(NULL);
    tm_tmp = localtime(&timer);
    
    /*
     * FIX: 15-Dec-2025 Daniel Liezrowice
     * Issue: BD-PB-NP and BD-PB-CHECKRETGEN - malloc() return value was not checked.
     * Parasoft Flow Analysis detected that the_record could be NULL if malloc fails,
     * and the subsequent memset() would dereference a null pointer causing undefined behavior.
     * Resolution: Added NULL check after malloc() and return NULL on allocation failure.
     * This prevents null pointer dereference and properly propagates allocation failures to caller.
     */
    the_record = (struct timer_record*)malloc(sizeof(struct timer_record));
    if (the_record == NULL) {
        return NULL;
    }
    memset(the_record, 0, sizeof(struct timer_record));
    
    /*
     * FIX: 15-Dec-2025 Daniel Liezrowice
     * Issue: BD-PB-MEMOPT and BD-RES-LEAKS - backup_record was allocated but never used or freed.
     * Parasoft Flow Analysis detected two problems:
     * 1. The memcpy call could be optimized out by the compiler since backup_record
     *    was never read after being written to (BD-PB-MEMOPT).
     * 2. The allocated memory for backup_record was never freed, causing a memory leak
     *    on the normal return path (BD-RES-LEAKS).
     * Resolution: Removed the entire backup_record allocation and memcpy operation
     * as it served no functional purpose. If backup functionality is needed in the future,
     * it should be properly implemented with a clear use case and proper cleanup.
     */
    
    /* starttime */
    print_string("Please enter the start hour [0-23] > ");
    start_h = get_input_digit();
    tm_tmp->tm_hour = start_h;
    
    /* BUG #9: INTEGER OVERFLOW
     * If user enters large values, multiplication can overflow.
     * Flow analysis tracks tainted data through arithmetic operations.
     */
    if (start_h > 0) {
        int seconds_offset = start_h * 3600 * 1000;  /* Potential overflow! */
        printf("Offset: %d\n", seconds_offset);
    }
    
    print_string("Please enter the start minute [0-59] > ");
    tm_tmp->tm_min = get_input_digit();
    
    the_record->starttime = mktime(tm_tmp);

    /* endtime */
    print_string("\nPlease enter the end hour [0-23] > ");
    tm_tmp->tm_hour = get_input_digit();
    print_string("\nPlease enter the end minute [0-59] > ");
    tm_tmp->tm_min = get_input_digit();
    
    the_record->endtime = mktime(tm_tmp);

    /* channel */
    print_string("\nPlease enter the channel to record > ");
    the_record->channel = get_input_digit();

    /* Cache for later use - sets up use-after-free bug */
    cached_record = the_record;
    
    return (the_record);
    /* backup_record is leaked here! */
}

int add_timer()
{
    struct timer_record* record;

    record = query_user();

    if (record) {
        add_timer_record(record);
    } else {
        return ERROR_CODE;
    }
    return 0;
}

void add_timer_record(struct timer_record* tr)
{
#ifdef OUTPUT
    {
        char[50] buf;
        sprintf(buf, "Curr Index = %d\n", curr_index);
        _EB_SEND(buf)
    }
#endif
    if (curr_index < max_records) {
        timer_records[curr_index++] = tr;
    } else {
        print_string("\nAll timers used ... timer not added\n");
    }
}

/*
 * Removed record at idx
 * Moves all records past idx, up one slot
 *
 * FIX: 15-Dec-2025 Daniel Liezrowice
 * Multiple issues were fixed in this function:
 *
 * Issue 1: BD-SECURITY-ARRAY - Tainted data from user input (idx parameter) was used
 *          directly as an array index without validation. If idx < 0 or idx >= max_records,
 *          this would cause out-of-bounds array access leading to undefined behavior.
 *          Resolution: Added bounds checking at function entry to validate idx is within
 *          valid range [0, max_records-1]. Function returns early if idx is invalid.
 *
 * Issue 2: BD-PB-ARRAY - The loop "for (i = idx-1; ...)" would access timer_records[-1]
 *          when idx=0, causing array out-of-bounds access at index -1.
 *          Resolution: Changed loop initialization from "i = idx-1" to "i = idx" to ensure
 *          the loop starts at a valid index and properly shifts records down.
 *
 * Issue 3: BD-SECURITY-ARRAY - The expression "timer_records[i+1]" could access index 100
 *          (out of bounds) when i=99 (max_records-1).
 *          Resolution: Changed loop condition to "i < curr_index - 1" to prevent i+1
 *          from exceeding the valid array bounds.
 *
 * Issue 4: Double-free bug - tr_copy was an alias to tr, and both were freed causing
 *          double-free undefined behavior.
 *          Resolution: Removed tr_copy variable and the conditional double-free code entirely.
 */
void delete_timer_record(int idx)
{
    struct timer_record* tr;
    int i;
    
    /* Bounds validation to prevent out-of-bounds array access */
    if (idx < 0 || idx >= max_records) {
        return;
    }
    
    tr = timer_records[idx];
    if (tr == NULL) {
        return;
    }
    
    /* Shift records down to fill the hole left by deleted record */
    for (i = idx; i < curr_index - 1; i++)
    {
        timer_records[i] = timer_records[i+1];
    }
    
    /* Clear the last slot and decrement count */
    if (curr_index > 0) {
        timer_records[curr_index - 1] = NULL;
        curr_index--;
    }
    
    free(tr);
}

/*
 * FIX: 15-Dec-2025 Daniel Liezrowice
 * Issue: BD-PB-CC - The condition "if (tr)" always evaluated to true because tr was
 *        already dereferenced in the strftime() calls above (tr->starttime, tr->endtime).
 *        Parasoft Flow Analysis detected this as a senseless condition - if tr was NULL,
 *        the program would have already crashed before reaching the if statement.
 * Resolution: Moved the NULL check to BEFORE any dereference of tr. Now the function
 *        returns early if tr is NULL, preventing null pointer dereference in strftime().
 *        Also added bounds checking for idx to prevent out-of-bounds array access.
 */
void format_timer_record(int idx, char* buf)
{
    char start[BUF_SIZE];
    char end[BUF_SIZE];
    struct timer_record* tr;
    
    /* Validate idx bounds and buf pointer */
    if (idx < 0 || idx >= max_records || buf == NULL) {
        return;
    }
    
    tr = timer_records[idx];
    
    /* Check tr BEFORE dereferencing to avoid null pointer access */
    if (tr == NULL) {
        return;
    }

    strftime(start, BUF_SIZE, "%I:%M %p", localtime(&tr->starttime));
    strftime(end, BUF_SIZE, "%I:%M %p", localtime(&tr->endtime));
    sprintf(buf, "%d\t%s\t%s\t%d\n", idx+1, start, end, tr->channel);
}

void list_timers()
{
    char buf[BUF_SIZE];
    int i;
    print_string("\n\nCurrent Set Timers");
    print_string("\nRecord#\tStart Time\tEnd Time\tChannel\n");
    for (i = 0; i < curr_index; i++)
    {
        format_timer_record(i, buf);
        print_string(buf);
    }
    print_string("\n\n");
}

