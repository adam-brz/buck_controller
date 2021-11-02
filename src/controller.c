#include "controller.h"

#include <avr/io.h>
#include <util/atomic.h>

#define Kp (10)
#define Ti (8)
#define Td (2)
#define Tp (1000LL / (F_CPU / 64LL / 256LL))

#define max_sum (2500)
#define min_sum (-2500)

#define Ki (Kp * Tp / Ti)
#define Kd (Kp * Td / Tp)

static volatile FixedPoint U_set = 0;
static volatile FixedPoint I_set = 0;

static volatile FixedPoint U_feedback = 0;
static volatile FixedPoint I_feedback = 0;

void controller_timer0_init(void)
{
	TCCR0B |= _BV(CS01) | _BV(CS00); // prescaler 64 (1 ms)
	TIMSK0 |= _BV(TOIE0); // enable overflow interrupt
}

void controller_set_inputs(const FixedPoint new_U, const FixedPoint new_I)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        U_set = new_U;
        I_set = new_I;
    }
}

void controller_set_feedback(const FixedPoint measured_U, const FixedPoint measured_I)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        U_feedback = measured_U;
        I_feedback = measured_I;
    }
}

// Regulator loop
ISR(TIMER0_OVF_vect)
{
    static int32_t prev_input_delta = 0;
    static int32_t integrator = 0;

    const int32_t delta = U_set - U_feedback;
    const int32_t diff = delta - prev_input_delta;

    integrator += delta * Ki;
    prev_input_delta = diff;

    if(integrator > max_sum)
        integrator = max_sum;
    else if(integrator < min_sum)
        integrator = min_sum;

    int32_t output = integrator + delta * Kp + diff * Kd;
    output = output / (3000 / 255);

    if(output > 255)
        output = 255;
    else if(output < 0)
        output = 0;

	OCR1A = output;
}