#if !defined(ADC_H)
#define ADC_H

#include <stdint.h>
#include "fixed_point.h"

#define ADC_MEASUREMENTS_COUNT 8
#define VREF 500 // fixed point 5.00 (2 digits)

void adc_init(void);
void adc_select_channel(uint8_t channel);

FixedPoint get_avg_adc(uint8_t channel);
FixedPoint get_voltage(void);
FixedPoint get_current(void);

#endif // ADC_H
