
#ifndef _clock_h_
#define _clock_h_

#include <stdint.h>

/* For embedded, use uint32_t for time representation */
typedef uint32_t time_t;

/* Display the uptime */
void display_time(void);

/* Set the time (placeholder) */
void set_time(time_t new_time);

/* Get current tick count */
uint32_t get_ticks(void);

/* Increment tick (call from SysTick_Handler) */
void tick_increment(void);

#endif /* _clock_h_ */

