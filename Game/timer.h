/** @file   timer.h
    @author yya139, zch65
    @date   21 October 2021
    @brief  timer header file for timer.c module
*/
#ifndef TIMER_H
#define TIMER_H

#include "system.h"

/* Initialise timer.  */
void timer_init (void);


/* Wait for the specified length of time.  */
void timer_delay_ms (uint16_t milliseconds);

#endif
