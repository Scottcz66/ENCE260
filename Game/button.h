/** @file   button.h
    @author yya139, zch65
    @date   21 October 2021
    @brief  button header file for button.c module
*/
#ifndef BUTTON_H
#define BUTTON_H


/** Return non-zero if button pressed.  */
int button_pressed_p (void);


/** Initialise button1.  */
void button_init (void);
#endif
