/*
 * fixed_point.h
 *
 * Created: 02.11.2021 11:15:25
 *  Author: Adam
 */


#ifndef FIXED_POINT_H_
#define FIXED_POINT_H_

#include <stdint.h>

typedef int16_t FixedPoint;

void fixed_point_to_str(FixedPoint number, char* output);

#endif /* FIXED_POINT_H_ */