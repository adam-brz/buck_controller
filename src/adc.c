#include "adc.h"

#include <avr/io.h>
#include <util/atomic.h>

static volatile uint16_t adc[2][ADC_MEASUREMENTS_COUNT];

void adc_init(void)
{
	DIDR0 = _BV(ADC0D) | _BV(ADC1D) | _BV(ADC2D) | _BV(ADC3D); // Disable digital on ADC pins

	ADMUX |= _BV(REFS0); // Vref = Vcc
	adc_select_channel(0);

	ADCSRA |= _BV(ADPS0) | _BV(ADPS2); // Prescaler 32
	ADCSRA |= _BV(ADEN) | _BV(ADIE) | _BV(ADSC); // Enable ADC, interrupts, start conversion (single)
}

void adc_select_channel(uint8_t channel)
{
	ADMUX = (ADMUX & 0xf0) | channel; // Select analog input (0.. 5)
}

FixedPoint get_avg_adc(uint8_t channel)
{
	uint32_t adc_sum = 0;

	for(uint8_t i = 0; i < ADC_MEASUREMENTS_COUNT; ++i)
	{
		ATOMIC_BLOCK(ATOMIC_FORCEON)
		{
			adc_sum += adc[channel][i];
		}
	}

	return adc_sum * VREF / ADC_MEASUREMENTS_COUNT / 1024UL;
}

FixedPoint get_voltage(void)
{
	return get_avg_adc(0);
}

FixedPoint get_current(void)
{
	return get_avg_adc(1);
}

ISR(ADC_vect)
{
	static uint8_t n = 0;
	static uint8_t channel = 0;

	// Store results
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		adc[channel][n] = ADC;
	}

	// Select second channel
	if(++n >= ADC_MEASUREMENTS_COUNT)
	{
		n = 0;
		channel = (channel + 1) % 2;
		adc_select_channel(channel);
	}

	// Start next conversion
	ADCSRA |= _BV(ADSC);
}