/*
 * lcd.c
 *
 *  Created on: 31.05.2012
 *      Author: Peter Schuster
 */

#include "xparameters.h"
#include "xbasic_types.h"
#include "xgpio.h"

#include "lcd.h"
#include "utils.h"

// Internal helpers.
void lcd_clk(void);
void lcd_set_rs(void);
void lcd_reset_rs(void);
void lcd_set_rw(void);
void lcd_reset_rw(void);

XGpio lcdIo;

// Write to GPIO outputs
void lcd_raw_write(Xuint32 c) {
  // Write to the GP IOs
  XGpio_DiscreteWrite(&lcdIo, 1, c & 0x0FF);
}

// Read the GPIO outputs
Xuint32 lcd_raw_read() {
  // Read from the GP IOs
  return XGpio_DiscreteRead(&lcdIo, 1);
}

void init_lcd() {
	Xuint32 temp;

	// Init gpio
	XGpio_Initialize(&lcdIo, XPAR_LCD_DEVICE_ID);
	// Set the direction for all signals to be outputs
	XGpio_SetDataDirection(&lcdIo, 1, 0x0);

	// Init lcd logic
	lcd_reset_rw(); // Write mode (always)
	lcd_reset_rs(); // Write control bytes

	// Delay 15ms
	delay_ms(15);

	// Initialize
	temp = lcd_raw_read();
	temp = temp | LCD_DB5;
	lcd_raw_write(temp);

	lcd_clk();
	lcd_clk();
	lcd_clk();

	// Delay 15ms
	delay_ms(15);

	// Function Set: 4 bit mode, 1/16 duty, 5x8 font, 2 lines
	lcd_write(0x28);
	// Display ON/OFF Control: ON
	lcd_write(0x0C);
	// Entry Mode Set: Increment (cursor moves forward)
	lcd_write(0x06);

	// Clear the display
	lcd_clear();
}

// Clock the LCD (toggles E)
void lcd_clk() {
  Xuint32 c;
  // Get existing outputs
  c = lcd_raw_read();
  delay_us(1);
  // Assert clock signal
  lcd_raw_write(c | LCD_E);
  delay_us(1);

  // De-assert the clock signal
  lcd_raw_write(c & (~LCD_E));
  delay_us(1);
}

// Assert the RS signal
void lcd_set_rs()
{
  Xuint32 c;
  // Get existing outputs
  c = lcd_raw_read();
  // Assert RS
  lcd_raw_write(c | LCD_RS);
  delay_us(1);
}

// Deassert the RS signal
void lcd_reset_rs()
{
  Xuint32 c;
  // Get existing outputs
  c = lcd_raw_read();
  // Assert RS
  lcd_raw_write(c & (~LCD_RS));
  delay_us(1);
}

// Assert the RW signal
void lcd_set_rw()
{
  Xuint32 c;
  // Get existing outputs
  c = lcd_raw_read();
  // Assert RS
  lcd_raw_write(c | LCD_RW);
  delay_us(1);
}

// Deassert the RW signal
void lcd_reset_rw()
{
  Xuint32 c;
  // Get existing outputs
  c = lcd_raw_read();
  // Assert RS
  lcd_raw_write(c & (~LCD_RW));
  delay_us(1);
}

// Write a byte to LCD (4 bit mode)
void lcd_write(Xuint32 c)
{
  Xuint32 temp;
  // Get existing outputs
  temp = lcd_raw_read();
  temp = temp & 0xF0;
  // Set the high nibble
  temp = temp | ((c >> 4) & 0x0F);
  lcd_raw_write(temp);
  // Clock
  lcd_clk();
  // Delay for "Write data into internal RAM 43us"
  delay_us(100);
  // Set the low nibble
  temp = temp & 0xF0;
  temp = temp | (c & 0x0F);
  lcd_raw_write(temp);
  // Clock
  lcd_clk();
  // Delay for "Write data into internal RAM 43us"
  delay_us(100);
}

// Clear LCD
void lcd_clear(void)
{
  lcd_reset_rs();
  // Clear LCD
  lcd_write(0x01);
  // Delay for "Clear display 1.53ms"
  delay_ms(2);
}

// Write a string to the LCD
void lcd_puts(const char * s) {
  lcd_set_rs();
  while(*s)
    lcd_write(*s++);
}

// Write character to the LCD
void lcd_putch(Xuint32 c) {
  lcd_set_rs();
  lcd_write(c);
}

// Change cursor position
// (line = 0 or 1, pos = 0 to 15)
void lcd_goto(Xuint32 line, Xuint32 pos) {
  lcd_reset_rs();
  pos = pos & 0x3F;
  if(line == 0)
    lcd_write(0x80 | pos);
  else
    lcd_write(0xC0 | pos);
}
