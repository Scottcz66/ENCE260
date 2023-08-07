/** @file   game.c
    @author yya139, zch65
    @date   21 October 2021
    @brief  Main script program for arrow game
*/

#include "system.h"
#include "pacer.h"
#include "navswitch.h"
#include "button.h"
#include "led.h"
#include "game_display.h"
#include "pio.h"
#include "timer.h"
#include "tinygl.h"
#include "../fonts/font5x7_1.h"


#define PACER_RATE 500
#define MESSAGE_RATE 10

#define NORTH 0
#define SOUTH 1
#define EAST 2
#define WEST 3
#define PUSH 4

#define PIEZO1_PIO PIO_DEFINE (PORT_D, 4)
#define PIEZO2_PIO PIO_DEFINE (PORT_D, 6)

#define TONE_FREQUENCY 440
#define LOOP_RATE (TONE_FREQUENCY * 2)

// Levels

static const uint8_t lvl1[] =
{
    NORTH, SOUTH, EAST, WEST, PUSH
};

static const uint8_t lvl2[] =
{
    EAST, PUSH, PUSH, WEST, NORTH, SOUTH, EAST, SOUTH, NORTH, PUSH
};

static const uint8_t lvl3[] =
{
    SOUTH, WEST, NORTH, NORTH, PUSH, EAST, WEST, EAST, PUSH, SOUTH, WEST, SOUTH, NORTH, PUSH
};

static const uint8_t lvl4[] =
{
    SOUTH, WEST, NORTH, WEST, SOUTH, NORTH, PUSH, EAST, PUSH, WEST, EAST, PUSH, SOUTH, WEST, EAST, SOUTH, SOUTH, NORTH, PUSH
};

static const uint8_t lvl5[] =
{
    EAST, WEST, EAST, PUSH, SOUTH, WEST, SOUTH, NORTH, PUSH, SOUTH, WEST, NORTH, NORTH, PUSH, EAST, PUSH, WEST, PUSH, SOUTH, SOUTH, NORTH, PUSH, PUSH, PUSH, PUSH
};

static const uint8_t lvl6[] =
{
    EAST, WEST, EAST, PUSH, SOUTH, WEST, WEST, NORTH, PUSH, PUSH, NORTH, PUSH, EAST, PUSH, WEST, PUSH, SOUTH, SOUTH, NORTH, PUSH, PUSH, SOUTH, NORTH, PUSH, SOUTH
};

static const uint8_t lvl7[] =
{
    NORTH, EAST, WEST, WEST, EAST, PUSH, SOUTH, NORTH, PUSH, PUSH, SOUTH, SOUTH, NORTH, PUSH, PUSH, EAST, PUSH, SOUTH, WEST, PUSH, SOUTH, WEST, NORTH,  PUSH, PUSH
};

// Main

int main (void)
{
    system_init ();
    pacer_init (PACER_RATE);
    button_init ();
    led_init ();
    tinygl_init (PACER_RATE); 
    tinygl_font_set (&font5x7_1); 
    tinygl_text_speed_set (MESSAGE_RATE);
    
    game_display_initialise ();
    timer_init ();
    
    char initial_level = '1';

    /*Initialise navigation switch driver.  */
    navswitch_init ();
    pio_config_set (PIEZO1_PIO, PIO_OUTPUT_LOW);
    pio_config_set (PIEZO2_PIO, PIO_OUTPUT_HIGH);

    pacer_init (LOOP_RATE);
    uint8_t running = 1;
    while(running)
    {
        pacer_wait ();
        led_on();
        tinygl_update ();
        pacer_wait ();
        pio_output_toggle (PIEZO1_PIO);
        pio_output_toggle (PIEZO2_PIO);
    
        
        
        
        
        
        
        /* Call the navswitch update function.  */
        navswitch_update ();
        
        /* Increment character if NORTH is pressed.  */
        if (navswitch_push_event_p (NAVSWITCH_NORTH)) {
            if (initial_level == '7') {
                initial_level = '1';
            } else {
                initial_level++;
            }

        }
        
        /* Decrement character if SOUTH is pressed.  */
        if (navswitch_push_event_p (NAVSWITCH_SOUTH)) {
            if (initial_level == '1') {
                initial_level = '7';
            } else {
                initial_level--;
            }
        }
        
        if (button_pressed_p()) {
            running = 0;
            led_off();
            pio_output_toggle(PIEZO2_PIO);
        }
        
        display_character (initial_level);
        
    }

    int16_t score = 0;
    while (1)
    {
        switch (initial_level) {
            case '1':
                timer_wait (500);
                for (uint8_t i = 0; i < 5; i++) {
                    score += display_direction (lvl1[i], 30);
                }
            case '2':
                timer_wait (500);
                for (uint8_t i = 0; i < 10; i++) {
                    score += display_direction (lvl2[i], 25);
                }
            case '3':
                timer_wait (500);
                for (uint8_t i = 0; i < 15; i++) {
                    score += display_direction (lvl3[i], 20);
                }
            case '4':
                timer_wait (500);
                for (uint8_t i = 0; i < 20; i++) {
                    score += display_direction (lvl4[i], 15);
                }
            case '5':
                timer_wait (500);
                for (uint8_t i = 0; i < 25; i++) {
                    score += display_direction (lvl5[i], 10);
                }
            case '6':
                timer_wait (500);
                for (uint8_t i = 0; i < 25; i++) {
                    score += display_direction (lvl6[i], 5);
                }
            case '7':
                timer_wait (500);
                for (uint8_t i = 0; i < 25; i++) {
                    score += display_direction (lvl7[i], 4);
                }

        }
        break;
    }
    tinygl_text_mode_set (TINYGL_TEXT_MODE_SCROLL);
    char snum[5];

    // convert score to string
    itoa(score, snum, 10);
    tinygl_text ("Game Over! You Scored...");
    
    while (1)
    {
        pacer_wait();
        tinygl_update ();

        if (button_pressed_p()){
            tinygl_text (snum);    //show the score
        }
    }
}

