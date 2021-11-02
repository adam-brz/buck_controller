#include "lcd_hd44780.h"

void lcd_init(void)
{
	hd44780_init();
	hd44780_outcmd(HD44780_CLR); // Clear DDRAM memory
	hd44780_wait_ready(1);
	hd44780_outcmd(HD44780_ENTMODE(1, 0));	// auto increment AC
	hd44780_wait_ready(1);
	hd44780_outcmd(HD44780_DISPCTL(1, 0, 0)); // Turn on display, off cursor
	hd44780_wait_ready(1);
}

void lcd_putchar(char c)
{
	static bool second_nl_seen;
	static uint8_t line = 0;

	if ((second_nl_seen) && (c != '\n') && (line == 0))
	{
		hd44780_wait_ready(40);
		hd44780_outcmd(HD44780_CLR);
		hd44780_wait_ready(1600);
		second_nl_seen=false;
	}
	if (c == '\n')
	{
		if (line==0)
		{
			line++;
			hd44780_outcmd(HD44780_DDADDR(64));	// Address of first char from second line
			hd44780_wait_ready(1000);
		}
		else
		{
			second_nl_seen=true;
			line=0;
		}
	}
	else
	{
		hd44780_outdata(c);
		hd44780_wait_ready(40);
	}
}

void lcd_puttext(const char *txt)
{
	char ch;
	while((ch=*txt))
	{
		lcd_putchar(ch);
		txt++;
	}
}

void lcd_putdata(const char *data, uint8_t dataSize)
{
	for (uint8_t i = 0; i < dataSize; ++i)
		lcd_putchar(data[i]);
}

void lcd_goto(uint8_t x, uint8_t y)
{
	hd44780_outcmd(HD44780_DDADDR(0x40*y+x));
	hd44780_wait_ready(1);
}

void lcd_cls(void)
{
	hd44780_outcmd(HD44780_CLR);
	hd44780_wait_ready(false);
}

void lcd_defchar(uint8_t charno, const uint8_t *chardef)
{
	hd44780_outcmd(HD44780_CGADDR(charno*8));
	hd44780_wait_ready(1);
	for (uint8_t c = 0; c < 8; ++c)
	{
		hd44780_outdata(*chardef++);
		hd44780_wait_ready(1);
	}
}