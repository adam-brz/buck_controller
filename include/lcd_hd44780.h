#ifndef LCD_HD44780_H
#define LCD_HD44780_H

#include <avr/pgmspace.h>
#include <stdbool.h>
#include <stdint.h>

#include "hd44780.h"

void lcd_init(void);

void lcd_putchar(char c);
void lcd_puttext(const char *txt);
void lcd_putdata(const char *data, uint8_t dataSize);

void lcd_goto(uint8_t x, uint8_t y);
void lcd_cls(void);

void lcd_defchar(uint8_t charno, const uint8_t *chardef);
#endif // LCD_HD44780_H