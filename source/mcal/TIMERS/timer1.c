/*
 * timer1.c
 *
 *  Created on: May 20, 2023
 *      Author: Abdelrahman
 */

#include "timer1.h"
#include "avr/io.h"


void Timer1_Init(Timer1_ConfigType * configType)
{
	TCCR1A |=
}
void Timer1_DeInit();
void Timer1_SetCallBack(void (*a_ptr) (void));
