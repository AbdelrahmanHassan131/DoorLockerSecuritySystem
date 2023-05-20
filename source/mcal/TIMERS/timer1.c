/*
 * timer1.c
 *
 *  Created on: May 20, 2023
 *      Author: Abdelrahman
 */

#include "timer1.h"
#include "avr/io.h"
#include "avr/interrupt.h"

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr)(void) = NULL_PTR; /*pointer to function*/

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/
/*Interrupt for compare mode*/
ISR(TIMER1_COMPA_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application */
		g_callBackPtr();
	}
}
/*Interrupt for Normal mode*/
ISR(TIMER1_OVF_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application */
		g_callBackPtr();
	}
}

void Timer1_Init(Timer1_ConfigType * configType)
{
	//TCCR1A |= ( (configType->mode & (0x03)) << 6) | (1<<FOC1A);

	if (configType->mode == TIMER1_MODE_COMPARE)
	{
		OCR1A = configType->compareValue;
		TIMSK |= (1<<OCIE1A);
	}
	else
	{
		TIMSK |= (1<<TOIE1);
	}

	TCCR1B |= ( configType->prescaler & (0x07) );
	TCCR1B |= ( (configType->mode & 0x01) << WGM12 );
	TCNT1 = configType->initialValue;
}
void Timer1_DeInit()
{
	TCCR1B = 0;
}
void Timer1_SetCallBack(void (*a_ptr) (void))
{
	if (a_ptr != NULL_PTR) {
		g_callBackPtr = a_ptr;
	}
}
