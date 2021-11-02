/*
 * fixed_point.c
 *
 * Created: 02.11.2021 11:15:40
 *  Author: Adam
 */ 

#include "fixed_point.h"
#include <stdio.h>

void fixed_point_to_str(FixedPoint number, char* output)
{
	const uint16_t full = number / 100;
	const uint16_t reminder = number % 100;
	
	const uint8_t written_size = sprintf(output, "%d.%.2d", full, reminder);
	output[written_size] = ' '; // Replace NULL with space
}