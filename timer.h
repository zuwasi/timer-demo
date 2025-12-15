
#ifndef _timer_h_
#define _timer_h_

#include <time.h>


/* timere structure */
struct timer_record
{
    time_t starttime;
    time_t endtime;
    unsigned channel;
};

/* init/uninit routines for the timer */
void init_timer();
void uninit_timer();

/* adds a timer, queries user for info, return ERROR_CODE on failure */
int add_timer();
    
/* adds a timer record */
void add_timer_record(struct timer_record*);

/* delete a timer */
void delete_timer_record(int);

/* get string for a single timer */
void format_timer_record(int, char*);

/* display list of all timers */
void list_timers();

/*
 * WATCHDOG TIMER API - 15-Dec-2025 Daniel Liezrowice
 * Software watchdog with 10 second expiration timeout
 */
void watchdog_init(void);           /* Initialize and start watchdog */
void watchdog_kick(void);           /* Reset watchdog to prevent expiration */
int  watchdog_check(void);          /* Check if watchdog expired (returns 1 if expired) */
void watchdog_disable(void);        /* Disable the watchdog */
int  watchdog_is_enabled(void);     /* Check if watchdog is enabled */
int  watchdog_time_remaining(void); /* Get seconds until expiration */

#endif /* _timer_h_ */

