/*
 * ultrasonic.c
 *
 *  Created on: Apr 28, 2023
 *      Author: Abdelrahman
 */

#include "gpio.h"
#include "ultrasonic.h"
#include "icu.h"
#include "util/delay.h"

uint16 Ultrasonic_distance = 0;
uint8 isDistanceCalculatedFlag = 0;
uint8 tick = 0;
uint16 t1 = 0;
uint16 t2 = 0;

void Ultrasonic_Init()
{
	Icu_ConfigType icuConfig = {F_CPU_8, RISING};
	Icu_init(&icuConfig);
	Icu_setCallBack(Ultrasonic_edgeProcessing);

	GPIO_setupPinDirection(PORTB_ID, PIN5_ID, PIN_OUTPUT);
	GPIO_writePin(PORTB_ID, PIN5_ID, LOGIC_LOW);

	Ultrasonic_distance = 0;
}

void Ultrasonic_Trigger()
{
	_delay_us(100);
	GPIO_writePin(PORTB_ID, PIN5_ID, LOGIC_HIGH);
	_delay_us(100);
	GPIO_writePin(PORTB_ID, PIN5_ID, LOGIC_LOW);
}
uint16 Ultrasonic_readDistance()
{
	Ultrasonic_Trigger();

	while(!isDistanceCalculatedFlag);

	isDistanceCalculatedFlag = 0;

	return Ultrasonic_distance;
}
void Ultrasonic_edgeProcessing()
{
	tick++;

	if (tick == 1)
	{
		t1 = Icu_getInputCaptureValue();
		Icu_setEdgeDetectionType(FALLING);
	}
	else if (tick == 2)
	{
		t2 = Icu_getInputCaptureValue();
		Ultrasonic_distance = Ultrasonic_Calc_Distance();
		Icu_clearTimerValue();
		t1 = 0;
		t2 = 0;
		tick = 0;
		isDistanceCalculatedFlag = 1;
		Icu_setEdgeDetectionType(RISING);
	}

}

uint16 Ultrasonic_Calc_Distance()
{
	uint16 timeTakenMicroSecond = (t2 -t1);
	uint16 distance = (timeTakenMicroSecond/10000.0) * (343.0/2.0);

	return distance;
}

