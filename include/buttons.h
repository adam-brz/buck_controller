#if !defined(BUTTONS_H)
#define BUTTONS_H

#include <stdbool.h>
#include <avr/io.h>

inline bool btn_I_plus_pressed(void)
{
    return !(PINB & _BV(PB2));
}

inline bool btn_I_minus_pressed(void)
{
    return !(PINB & _BV(PB3));
}

inline bool btn_U_plus_pressed(void)
{
    return !(PINB & _BV(PB4));
}

inline bool btn_U_minus_pressed(void)
{
    return !(PINB & _BV(PB5));
}

inline bool btn_SET_pressed(void)
{
    return !(PIND & _BV(PD7));
}

#endif // BUTTONS_H
