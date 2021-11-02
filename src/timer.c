#include "timer.h"

#include <avr/io.h>
#include <avr/interrupt.h>

extern uint8_t lcd_timer;
extern uint8_t btn_timer;
extern uint8_t measure_timer;

void timer2_init(void)
{
	TCCR2B |= _BV(CS22); // prescaler 64 (around 1 ms)
	TIMSK2 |= _BV(TOIE2); // enable overflow interrupt
}

ISR(TIMER2_OVF_vect)
{
	lcd_timer++;
	btn_timer++;
	measure_timer++;
}