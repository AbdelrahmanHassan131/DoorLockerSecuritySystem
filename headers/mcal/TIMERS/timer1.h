/*
 * timer1.h
 *
 *  Created on: May 20, 2023
 *      Author: Abdelrahman
 */

#ifndef HEADERS_MCAL_TIMERS_TIMER1_H_
#define HEADERS_MCAL_TIMERS_TIMER1_H_

#include "std_types.h"
typedef enum {
	TIMER1_PRESCALER_NONE,
	TIMER1_PRESCALER_1,
	TIMER1_PRESCALER_8,
	TIMER1_PRESCALER_64,
	TIMER1_PRESCALER_256,
	TIMER1_PRESCALER_1024
}Timer1_Prescaler;

typedef enum {
	TIMER1_MODE_NORMAL,
	TIMER1_MODE_COMPARE
}Timer1_Mode;

typedef struct {
	uint16 initialValue;
	uint16 compareValue;
	Timer1_Prescaler prescaler;
	Timer1_Mode mode;
}Timer1_ConfigType;


void Timer1_Init(Timer1_ConfigType * configType);
void Timer1_DeInit();
void Timer1_SetCallBack(void (*a_ptr) (void));


#endif /* HEADERS_MCAL_TIMERS_TIMER1_H_ */
