/*
* buck_controller.c
*
* Created: 27.10.2021 23:52:03
* Author : Adam
*/

#include <stdbool.h>

#include <avr/io.h>
#include <util/delay.h>
#include <util/atomic.h>

#include "adc.h"
#include "buttons.h"
#include "controller.h"
#include "pwm_generator.h"
#include "timer.h"
#include "lcd_hd44780.h"
#include "fixed_point.h"

#define BUTTON_DELAY (200/1)
#define LCD_DELAY (500/2)

volatile uint8_t lcd_timer = 0;
volatile uint8_t btn_timer = 0;
volatile uint8_t measure_timer = 0;

int main(void)
{
	static FixedPoint U_set = 0;
	static FixedPoint I_set = 0;

	static FixedPoint U_measured = 0;
	static FixedPoint I_measured = 0;

	static uint16_t button_speed = 5;
	static bool blink = false;
	static bool voltage_changed = false;
	static bool current_changed = false;

	// Unused pins (configure as input with pull up)
	PORTB |= _BV(PB0);
	PORTC |= _BV(PC2) | _BV(PC3);

	// LEDs
	DDRC |= _BV(PC4) | _BV(PC5);
	PORTC |= _BV(PC4);

	// Buttons
	PORTD |= _BV(PD7);
	PORTB |= _BV(PB2) | _BV(PB3) | _BV(PB4) | _BV(PB5);

	// PWM signal output
	DDRB |= _BV(PB1);

	controller_timer0_init();
	pwm_timer1_init();
	timer2_init();

	lcd_init();
	adc_init();

	// Enable interrupts
	sei();

	while (1)
	{
		if(btn_I_plus_pressed())
		{
			if(btn_timer >= BUTTON_DELAY)
			{
				btn_timer = 0;
				current_changed = true;

				I_set += button_speed;

				if(I_set > 500)
					I_set = 500;

				button_speed += 1;
			}
		}
		else if (btn_I_minus_pressed())
		{
			if(btn_timer >= BUTTON_DELAY)
			{
				btn_timer = 0;
				current_changed = true;

				if(I_set < button_speed)
					I_set = 0;
				else
					I_set -= button_speed;

				button_speed += 1;
			}
		}
		else if (btn_U_plus_pressed())
		{
			if(btn_timer >= BUTTON_DELAY)
			{
				btn_timer = 0;
				voltage_changed = true;

				U_set += button_speed;

				if(U_set > 3000)
					U_set = 3000;

				button_speed += 1;
			}

		}
		else if (btn_U_minus_pressed())
		{
			if(btn_timer >= BUTTON_DELAY)
			{
				btn_timer = 0;
				voltage_changed = true;

				if(U_set < button_speed)
					U_set = 0;
				else
					U_set -= button_speed;

				button_speed += 1;
			}
		}
		else
		{
			button_speed = 5;
		}

		if(button_speed > 2000)
			button_speed = 2000;

		if (btn_SET_pressed() && btn_timer >= BUTTON_DELAY)
		{
			btn_timer = 0;

			voltage_changed = false;
			current_changed = false;

			controller_set_inputs(U_set, I_set);
		}

		if(measure_timer >= 1)
		{
			measure_timer = 0;
			U_measured = get_voltage();
			I_measured = get_current();
			controller_set_feedback(U_measured, I_measured);
		}

		if(lcd_timer >= LCD_DELAY)
		{
			lcd_timer = 0;
			blink = !blink;

			char voltage_info[16] = "U:              ";
			char current_info[16] = "I:              ";

			fixed_point_to_str(U_measured, &voltage_info[3]);

			if(!(blink && voltage_changed))
				fixed_point_to_str(U_set, &voltage_info[9]);

			fixed_point_to_str(I_measured, &current_info[3]);

			if(!(blink && current_changed))
				fixed_point_to_str(I_set, &current_info[9]);

			lcd_goto(0, 0);
			lcd_puttext(voltage_info);

			lcd_goto(0, 1);
			lcd_puttext(current_info);
		}
	}
}
