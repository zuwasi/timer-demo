
#ifndef _timer_h_
#define _timer_h_

#include <stdint.h>

/* Timer structure for embedded - no time_t dependency */
struct timer_record
{
    uint8_t start_hour;
    uint8_t start_min;
    uint8_t end_hour;
    uint8_t end_min;
    uint8_t channel;
};

/* init/uninit routines for the timer */
void init_timer(void);
void uninit_timer(void);

/* adds a timer, queries user for info, return ERROR_CODE on failure */
int add_timer(void);
    
/* adds a timer record */
void add_timer_record(struct timer_record *tr);

/* delete a timer */
void delete_timer_record(int idx);

/* get string for a single timer */
void format_timer_record(int idx, char *buf);

/* display list of all timers */
void list_timers(void);

#endif /* _timer_h_ */

