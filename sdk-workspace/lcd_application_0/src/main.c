/*
 * main.c
 *
 *  Created on: 31.05.2012
 *      Author: Peter Schuster
 */

#include <stdio.h>
#include "platform.h"
#include "xparameters.h"
#include "xbasic_types.h"
#include "xgpio.h"
#include "xstatus.h"
#include "xtmrctr.h"
#include "lcd.h"
#include "utils.h"

void print(char *str);

int main() {
	Xuint32 i;

	init_platform();
	print("Initialized Platform.\r\n");

	print("Initializing Utils...\r\n");
	init_utils();
	print("Initializing LCD...\r\n");
	init_lcd();

	print("Ready.\r\n");

	lcd_goto(0,2);
	lcd_puts("HTTP Project");
	lcd_goto(1,1);
	lcd_puts("Peter Schuster");

	// Wait 30 seconds before shutdown
	for (i=0; i < 30; i++)
		delay_ms(1000);

	lcd_clear();
	lcd_goto(0,3);
	lcd_puts("Goodbye");

	delay_ms(2000);
	lcd_clear();

	print("Shutting down...\r\n");
	cleanup_platform();

	return 0;
}
