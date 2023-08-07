/** @file   game_display.c
    @author yya139, zch65
    @date   21 October 2021
    @brief  game display module for game display (matrix gird led) functions
*/
#include "system.h"
#include "pacer.h"
#include "pio.h"
#include "navswitch.h"
#include "game_display.h"


#define NORTH 0
#define SOUTH 1
#define EAST 2
#define WEST 3
#define PUSH 4

/** Define PIO pins driving LED matrix rows.  */
static const pio_t rows[] =
{
    LEDMAT_ROW1_PIO, LEDMAT_ROW2_PIO, LEDMAT_ROW3_PIO,
    LEDMAT_ROW4_PIO, LEDMAT_ROW5_PIO, LEDMAT_ROW6_PIO,
    LEDMAT_ROW7_PIO
};


/** Define PIO pins driving LED matrix columns.  */
static const pio_t cols[] =
{
    LEDMAT_COL1_PIO, LEDMAT_COL2_PIO, LEDMAT_COL3_PIO,
    LEDMAT_COL4_PIO, LEDMAT_COL5_PIO
};

// Bit Maps

static const uint8_t left1[] =           //Left
{
  0x00, 0x00, 0x01, 0x00, 0x00
};

static const uint8_t left2[] =           //Left
{
  0x00, 0x01, 0x03, 0x01, 0x01
};

static const uint8_t left3[] =           //Left
{
  0x00, 0x02, 0x07, 0x02, 0x02

};

static const uint8_t left4[] =           //Left
{
  0x00, 0x04, 0x0E, 0x04, 0x04
};

static const uint8_t left5[] =           //Left
{
  0x00, 0x08, 0x1C, 0x08, 0x08
};

static const uint8_t left6[] =           //Left
{
  0x00, 0x10, 0x38, 0x10, 0x10
};

static const uint8_t left7[] =           //Left
{
  0x00, 0x20, 0x70, 0x20, 0x20
};

static const uint8_t right1[] =              //Right
{
  0x00, 0x00, 0x01, 0x00, 0x00  //1
};

static const uint8_t right2[] =              //Right
{
  0x01, 0x01, 0x03, 0x01, 0x00
};

static const uint8_t right3[] =              //Right
{
  0x02, 0x02, 0x07, 0x02, 0x00
};

static const uint8_t right4[] =              //Right
{
  0x04, 0x04, 0x0E, 0x04, 0x00
};

static const uint8_t right5[] =              //Right
{
  0x08, 0x08, 0x1C, 0x08, 0x00
};

static const uint8_t right6[] =              //Right
{
  0x10, 0x10, 0x38, 0x10, 0x00
};

static const uint8_t right7[] =              //Right
{
  0x20, 0x20, 0x70, 0x20, 0x00
};

static const uint8_t top1[] =       //Top
{
   0x00, 0x00, 0x01, 0x00, 0x00
};

static const uint8_t top2[] =      //Top
{
   0x00, 0x00, 0x03, 0x00, 0x00
};

static const uint8_t top3[] =      //Top
{
   0x00, 0x01, 0x07, 0x01, 0x00
};

static const uint8_t top4[] =      //Top
{
   0x00, 0x02, 0x0F, 0x02, 0x00
};

static const uint8_t top5[] =      //Top
{
   0x00, 0x04, 0x1E, 0x04, 0x00
};

static const uint8_t top6[] =      //Top
{
   0x00, 0x08, 0x3C, 0x08, 0x00
};
static const uint8_t top7[] =      //Top
{
   0x00, 0x10, 0x78, 0x10, 0x00
};


static const uint8_t down1[] =      //Down
{
    0x00, 0x00, 0x01, 0x00, 0x00
};

static const uint8_t down2[] =      //Down
{
    0x00, 0x01, 0x03, 0x01, 0x00
};

static const uint8_t down3[] =      //Down
{
    0x00, 0x02, 0x07, 0x02, 0x00
};

static const uint8_t down4[] =      //Down
{
    0x00, 0x04, 0x0F, 0x04, 0x00
};

static const uint8_t down5[] =      //Down
{
    0x00, 0x08, 0x1E, 0x08, 0x00
};

static const uint8_t down6[] =      //Down
{
    0x00, 0x10, 0x3C, 0x10, 0x00
};

static const uint8_t down7[] =      //Down
{
    0x00, 0x20, 0x78, 0x20, 0x00
};

static const uint8_t push1[] =      //Push
{
   0x00, 0x00, 0x01, 0x00, 0x00
};

static const uint8_t push2[] =      //Push
{
   0x00, 0x01, 0x03, 0x01, 0x00
};

static const uint8_t push3[] =      //Push
{
  0x00, 0x02, 0x07, 0x02, 0x00
};
static const uint8_t push4[] =      //Push
{
  0x00, 0x04, 0x0E, 0x04, 0x00
};

static const uint8_t push5[] =      //Push
{
  0x00, 0x08, 0x1C, 0x08, 0x00
};

static const uint8_t push6[] =      //Push
{
  0x00, 0x10, 0x38, 0x10, 0x00
};
static const uint8_t push7[] =      //Push
{
  0x00, 0x20, 0x70, 0x20, 0x00
};

static const uint8_t hit[] =        //Hit
{
    0x30, 0x46, 0x40, 0x46, 0x30
};

static const uint8_t miss[] =       //Miss
{
    0x00, 0x46, 0x40, 0x46, 0x00
};

//Functions

void display_character (char character)
{
    char buffer[2];

    buffer[0] = character;
    buffer[1] = '\0';
    tinygl_text (buffer);
}

void game_display_initialise ()
{
    for (uint8_t i = 0; i < 7; i++) {
        pio_config_set (rows[i], PIO_OUTPUT_HIGH);
    }
    for (uint8_t i = 0; i < 5; i++) {
        pio_config_set (cols[i], PIO_OUTPUT_HIGH);
    }
}

void display_column (uint8_t row_pattern, uint8_t current_column)
{
    static pio_t previous_column = 0;

    pio_output_high (cols[previous_column]);
    previous_column = current_column;

    for (uint8_t current_row = 0; current_row < 7; current_row++) {
        if ((row_pattern >> current_row) & 1)
        {
            pio_output_low (rows[current_row]);
        }
        else
        {
            pio_output_high (rows[current_row]);
        }
    }

    pio_output_low (cols[current_column]);
}


int16_t display_bitmap(uint8_t gamespeed, uint8_t direction, const uint8_t* bitmap, uint8_t end){
    int16_t score = 0;
    uint8_t current_column = 0;

    for (uint16_t x = 0; x < gamespeed*10; x++)
    {
        pacer_wait ();
        navswitch_update ();

        display_column (bitmap[current_column], current_column);

        current_column++;

        if (current_column > (LEDMAT_COLS_NUM - 1))
        {
            current_column = 0;
        }

        if (navswitch_push_event_p (NAVSWITCH_NORTH))
        {
            if (direction == NORTH && end) {
                display_bitmap(30, 5, hit, 0);
                score += 20;
            } else {
                display_bitmap(30, 5, miss, 0);
                score -= 5;
            }
            break;
        }
        if (navswitch_push_event_p (NAVSWITCH_EAST)) {
            if (direction == EAST && end) {
                display_bitmap(30, 5, hit, 0);
                score += 20;
            } else {
                display_bitmap(30, 5, miss, 0);
                score -= 5;
            }
            break;
        }
        if (navswitch_push_event_p (NAVSWITCH_SOUTH)) {
            if (direction == SOUTH && end) {
                display_bitmap(30, 5, hit, 0);
                score += 20;
            } else {
                display_bitmap(30, 5, miss, 0);
                score -= 5;
            }
            break;
        }
        if (navswitch_push_event_p (NAVSWITCH_WEST)) {
            if (direction == WEST && end) {
                display_bitmap(30, 5, hit, 0);
                score += 20;
            } else {
                display_bitmap(30, 5, miss, 0);
                score -= 5;
            }
            break;
        }
        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            if (direction == PUSH && end) {
                display_bitmap(30, 5, hit, 0);
                score += 20;
            } else {
                display_bitmap(30, 5, miss, 0);
                score -= 5;
            }
            break;
        }
    }
    return score;
}

int16_t display_direction (uint8_t x, uint8_t gamespeed) {
    int16_t score = 0;
    switch (x)
    {
        case NORTH: // 0
            score += display_bitmap (gamespeed, NORTH, top1, 0);
            score += display_bitmap (gamespeed, NORTH, top2, 0);
            score += display_bitmap (gamespeed, NORTH, top3, 0);
            score += display_bitmap (gamespeed, NORTH, top4, 0);
            score += display_bitmap (gamespeed, NORTH, top5, 0);
            score += display_bitmap (gamespeed, NORTH, top6, 0);
            score += display_bitmap (gamespeed, NORTH, top7, 1);
            break;
        case EAST: // 1
            score += display_bitmap (gamespeed, EAST, right1, 0);
            score += display_bitmap (gamespeed, EAST, right2, 0);
            score += display_bitmap (gamespeed, EAST, right3, 0);
            score += display_bitmap (gamespeed, EAST, right4, 0);
            score += display_bitmap (gamespeed, EAST, right5, 0);
            score += display_bitmap (gamespeed, EAST, right6, 0);
            score += display_bitmap (gamespeed, EAST, right7, 1);
            break;
        case SOUTH: // 2
            score += display_bitmap (gamespeed, SOUTH, down1, 0);
            score += display_bitmap (gamespeed, SOUTH, down2, 0);
            score += display_bitmap (gamespeed, SOUTH, down3, 0);
            score += display_bitmap (gamespeed, SOUTH, down4, 0);
            score += display_bitmap (gamespeed, SOUTH, down5, 0);
            score += display_bitmap (gamespeed, SOUTH, down6, 0);
            score += display_bitmap (gamespeed, SOUTH, down7, 1);
            break;
        case WEST: // 3
            score += display_bitmap (gamespeed, WEST, left1, 0);
            score += display_bitmap (gamespeed, WEST, left2, 0);
            score += display_bitmap (gamespeed, WEST, left3, 0);
            score += display_bitmap (gamespeed, WEST, left4, 0);
            score += display_bitmap (gamespeed, WEST, left5, 0);
            score += display_bitmap (gamespeed, WEST, left6, 0);
            score += display_bitmap (gamespeed, WEST, left7, 1);
            break;
        case PUSH: // 4
            score += display_bitmap (gamespeed, PUSH, push1, 0);
            score += display_bitmap (gamespeed, PUSH, push2, 0);
            score += display_bitmap (gamespeed, PUSH, push3, 0);
            score += display_bitmap (gamespeed, PUSH, push4, 0);
            score += display_bitmap (gamespeed, PUSH, push5, 0);
            score += display_bitmap (gamespeed, PUSH, push6, 0);
            score += display_bitmap (gamespeed, PUSH, push7, 1);
            break;
    }
    return score;
}


