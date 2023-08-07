/** @file   timer.c
    @author yya139, zch65
    @date   21 October 2021
    @brief  timer module for timer functions
*/

#include <avr/io.h>
#include "timer.h"

/* Initialise timer.  */
void timer_init (void)
{
    TCCR1A = 0x00;
    TCCR1B = 0x05;
    TCCR1C = 0x00;

}


/* Wait for the specified length of time.  */
void timer_delay_ms (uint16_t milliseconds)
{

    /* Calculate the timer/counter value needed 
       for the given number of milliseconds. */
    uint16_t ticks = milliseconds * (8000000/1024) / 1000; 

    /* Wait for the timer/couter to reach the 
       value calculated above.  */
    TCNT1 = 0;
    while (TCNT1 < ticks) {
        continue;        
    }

}
