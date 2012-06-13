/*
 * lcd.h
 *
 *  Created on: 31.05.2012
 *      Author: Peter Schuster
 */

#ifndef LCD_H_
#define LCD_H_

#include "xbasic_types.h"

// Masks to the pins on the GPIO port

#define LCD_DB4    0x01
#define LCD_DB5    0x02
#define LCD_DB6    0x04
#define LCD_DB7    0x08
#define LCD_RW     0x10
#define LCD_RS     0x20
#define LCD_E      0x40
#define LCD_TEST   0x80

void lcd_set_test(void);
void lcd_reset_test(void);
void lcd_write(Xuint32 c);
void lcd_clear(void);
void lcd_puts(const char * s);
void lcd_putch(Xuint32 c);

/**
 * Change cursor position
 * (line = 0 or 1, pos = 0 to 15)
 */
void lcd_goto(Xuint32 line, Xuint32 pos);
void init_lcd(void);

#endif /* LCD_H_ */
