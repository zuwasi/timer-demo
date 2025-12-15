
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
    int start_h, start_m, end_h, end_m;
    struct timer_record* the_record;
    struct timer_record* backup_record;  /* BUG #3: Memory leak on error path */
    time_t timer;
    struct tm* tm_tmp;

    timer = time(NULL);
    tm_tmp = localtime(&timer);
    
    the_record = (struct timer_record*)malloc(sizeof(struct timer_record));
    memset(the_record, 0, sizeof(struct timer_record));
    
    /* BUG #3: MEMORY LEAK ON ERROR PATH
     * backup_record is allocated but never freed if we return normally.
     * Flow analysis detects that this allocation leaks on the normal return path.
     */
    backup_record = (struct timer_record*)malloc(sizeof(struct timer_record));
    if (backup_record) {
        memcpy(backup_record, the_record, sizeof(struct timer_record));
    }
    
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
 */
void delete_timer_record(int idx)
{
    struct timer_record* tr = timer_records[idx];
    struct timer_record* tr_copy = tr;  /* BUG #7: Sets up double-free */
    int i;
    
    /* fill in the holes */
    for (i = idx-1; i < curr_index; i++)
    {
        if (0 == timer_records[i]) {
            break;
        } else {
            timer_records[i] = timer_records[i+1];
        }
    }
    curr_index--;
    free(tr);
    
    /* BUG #7: DOUBLE FREE
     * tr_copy still points to the same memory as tr.
     * Freeing tr_copy after tr causes double-free.
     * Flow analysis tracks pointer aliasing to detect this.
     */
    if (idx == 0 && tr_copy != NULL) {
        free(tr_copy);  /* Double free! tr_copy aliases tr */
    }
}

void format_timer_record(int idx, char* buf)
{
    char start[BUF_SIZE];
    char end[BUF_SIZE];
    
    struct timer_record* tr = timer_records[idx];

    strftime(start, BUF_SIZE, "%I:%M %p", localtime(&tr->starttime));
    strftime(end, BUF_SIZE, "%I:%M %p", localtime(&tr->endtime));

    if (tr) {
        sprintf(buf, "%d\t%s\t%s\t%d\n", idx+1, start, end, tr->channel);
    }
    
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

