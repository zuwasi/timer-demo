/*
 * Clock related functions for STM32WB5x embedded version
 * Uses a simple tick counter instead of system time
 */

#include "clock.h"
#include "consts.h"
#include "inout.h"

/* Simple tick counter (incremented by SysTick if configured) */
static volatile uint32_t system_ticks = 0;

/*
 * Get current tick count
 */
uint32_t get_ticks(void)
{
    return system_ticks;
}

/*
 * Increment tick (call from SysTick_Handler)
 */
void tick_increment(void)
{
    system_ticks++;
}

/*
 * Print the current time (simplified for embedded)
 */
void display_time(void)
{
    char buf[BUF_SIZE];
    uint32_t ticks = get_ticks();
    uint32_t seconds = ticks / 1000;
    uint32_t minutes = seconds / 60;
    uint32_t hours = minutes / 60;
    
    /* Format: HH:MM:SS */
    buf[0] = '\n';
    buf[1] = '\n';
    buf[2] = 'U';
    buf[3] = 'p';
    buf[4] = 't';
    buf[5] = 'i';
    buf[6] = 'm';
    buf[7] = 'e';
    buf[8] = ':';
    buf[9] = ' ';
    buf[10] = '0' + (char)((hours / 10) % 10);
    buf[11] = '0' + (char)(hours % 10);
    buf[12] = ':';
    buf[13] = '0' + (char)((minutes % 60) / 10);
    buf[14] = '0' + (char)(minutes % 10);
    buf[15] = ':';
    buf[16] = '0' + (char)((seconds % 60) / 10);
    buf[17] = '0' + (char)(seconds % 10);
    buf[18] = '\n';
    buf[19] = '\n';
    buf[20] = '\0';
    
    print_string(buf);
}

/* 
 * Dummy Function -- time always taken from tick counter
 */
void set_time(time_t new_time)
{
    (void)new_time;
}
