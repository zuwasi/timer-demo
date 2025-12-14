/*
 * Timer class, stores timer records
 * STM32WB5x Embedded Version - No stdlib dependencies
 */

#include <stdint.h>
#include <stddef.h>

#include "clock.h"
#include "consts.h"
#include "inout.h"
#include "timer.h"

static struct timer_record timer_storage[BUF_SIZE];
static struct timer_record* timer_records[BUF_SIZE];
const int max_records = BUF_SIZE;
static int curr_index = 0;

/* Simple memset for embedded */
static void embedded_memset(void *ptr, int value, size_t num)
{
    unsigned char *p = (unsigned char *)ptr;
    while (num--) {
        *p++ = (unsigned char)value;
    }
}

void init_timer(void)
{
    int i;
    for (i = 0; i < BUF_SIZE; i++) {
        timer_records[i] = NULL;
    }
    curr_index = 0;
}

void uninit_timer(void)
{
    int i;
    for (i = 0; i < max_records; i++) {
        timer_records[i] = NULL;
    }
    curr_index = 0;
}

/*
 * Queries user for timer information
 * Returns pointer to statically allocated timer record
 */
struct timer_record* query_user(void)
{
    struct timer_record* the_record;
    
    if (curr_index >= max_records) {
        return NULL;
    }
    
    the_record = &timer_storage[curr_index];
    embedded_memset(the_record, 0, sizeof(struct timer_record));
    
    /* starttime */
    print_string("Please enter the start hour [0-23] > ");
    the_record->start_hour = (uint8_t)get_input_digit();
    print_string("Please enter the start minute [0-59] > ");
    the_record->start_min = (uint8_t)get_input_digit();

    /* endtime */
    print_string("\nPlease enter the end hour [0-23] > ");
    the_record->end_hour = (uint8_t)get_input_digit();
    print_string("\nPlease enter the end minute [0-59] > ");
    the_record->end_min = (uint8_t)get_input_digit();

    /* channel */
    print_string("\nPlease enter the channel to record > ");
    the_record->channel = (uint8_t)get_input_digit();

    return the_record;
}

int add_timer(void)
{
    struct timer_record* record;

    record = query_user();

    if (record != NULL) {
        add_timer_record(record);
        return 0;
    }
    return ERROR_CODE;
}

void add_timer_record(struct timer_record* tr)
{
    if (tr == NULL) {
        return;
    }
    
    if (curr_index >= 0 && curr_index < max_records) {
        timer_records[curr_index++] = tr;
    } else {
        print_string("\nAll timers used ... timer not added\n");
    }
}

/*
 * Removes record at idx
 */
void delete_timer_record(int idx)
{
    int i;
    
    /* Validate index bounds */
    if (idx < 0 || idx >= max_records || idx >= curr_index) {
        print_string("Invalid timer index\n");
        return;
    }
    
    /* Shift records down */
    for (i = idx; i < curr_index - 1; i++) {
        timer_records[i] = timer_records[i + 1];
    }
    
    timer_records[curr_index - 1] = NULL;
    curr_index--;
}

/* Helper to convert number to string */
static void int_to_str(int num, char *buf, int width)
{
    int i;
    for (i = width - 1; i >= 0; i--) {
        buf[i] = '0' + (num % 10);
        num /= 10;
    }
}

void format_timer_record(int idx, char* buf)
{
    struct timer_record* tr;
    
    if (buf == NULL) {
        return;
    }
    
    if (idx < 0 || idx >= max_records || idx >= curr_index) {
        buf[0] = '\0';
        return;
    }
    
    tr = timer_records[idx];

    if (tr != NULL) {
        int pos = 0;
        
        /* Record number */
        buf[pos++] = '0' + (char)((idx + 1) % 10);
        buf[pos++] = '\t';
        
        /* Start time HH:MM */
        int_to_str(tr->start_hour, &buf[pos], 2);
        pos += 2;
        buf[pos++] = ':';
        int_to_str(tr->start_min, &buf[pos], 2);
        pos += 2;
        buf[pos++] = '\t';
        
        /* End time HH:MM */
        int_to_str(tr->end_hour, &buf[pos], 2);
        pos += 2;
        buf[pos++] = ':';
        int_to_str(tr->end_min, &buf[pos], 2);
        pos += 2;
        buf[pos++] = '\t';
        
        /* Channel */
        buf[pos++] = '0' + (char)(tr->channel % 10);
        buf[pos++] = '\n';
        buf[pos] = '\0';
    } else {
        buf[0] = '\0';
    }
}

void list_timers(void)
{
    char buf[BUF_SIZE];
    int i;
    
    print_string("\n\nCurrent Set Timers");
    print_string("\nRec#\tStart\tEnd\tCh\n");
    
    for (i = 0; i < curr_index; i++) {
        format_timer_record(i, buf);
        print_string(buf);
    }
    print_string("\n\n");
}
