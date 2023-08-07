/** @file   led.c
    @author yya139, zch65
    @date   21 October 2021
    @brief  led module for led functions
*/
#include "pio.h"
#include "led.h"

/** Turn LED1 on.  */
void led_on (void)
{
    pio_output_high (PC2_PIO);
}


/** Turn LED1 off.  */
void led_off (void)
{
    pio_output_low (PC2_PIO);
}

/** Toggle LED on/off.  */
void led_toggle (void)
{
    pio_output_toggle (PC2_PIO);
}

/** Initialise LED1.  */
void led_init (void)
{
    pio_config_set (PC2_PIO, PIO_OUTPUT_LOW);
}
