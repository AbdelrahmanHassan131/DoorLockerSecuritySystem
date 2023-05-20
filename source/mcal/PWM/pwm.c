/*
 * pwm.c
 *
 *  Created on: Apr 8, 2023
 *      Author: Abdelrahman
 */

#include "pwm.h"
#include "avr/io.h"

void PWM_TIMER0_Start(uint8 duty)
{
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01);
	OCR0 = (uint8)((duty/100.0) * 255.0);
}
