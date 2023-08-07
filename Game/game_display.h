/** @file   game_display.h
    @author yya139, zch65
    @date   21 October 2021
    @brief  game_display header file for game_display.c module
*/
#ifndef GAME_DISPLAY_H
#define GAME_DISPLAY_H

void display_character (char character);

void game_display_initialise (void);

void display_column (uint8_t row_pattern, uint8_t current_column);

int16_t display_bitmap(uint8_t gamespeed, uint8_t direction, const uint8_t* bitmap, uint8_t end);

int16_t display_direction (uint8_t x, uint8_t gamespeed);

#endif // GAME_DISPLAY_H
