#if !defined(PWN_GENERATOR_H)
#define PWN_GENERATOR_H

#include <avr/io.h>

inline void pwm_timer1_init(void)
{
	// Timer for generating PWM signal
	TCCR1A |= _BV(COM1A1) | _BV(COM1A0) | _BV(WGM10); // inverting mode
	TCCR1B |= _BV(WGM12) | _BV(CS10);	 // prescaler 1 (64kHz), fast PWM
	OCR1A = 0;
}

#endif // PWN_GENERATOR_H
