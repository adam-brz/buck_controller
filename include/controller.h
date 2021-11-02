#if !defined(CONTROLLER_H)
#define CONTROLLER_H

#include "fixed_point.h"

void controller_timer0_init(void);
void controller_set_inputs(const FixedPoint new_U, const FixedPoint new_I);
void controller_set_feedback(const FixedPoint measured_U, const FixedPoint measured_I);

#endif // CONTROLLER_H
