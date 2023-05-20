/*
 * buzzer.c
 *
 *  Created on: May 20, 2023
 *      Author: Abdelrahman
 */

#include "buzzer.h"
#include "gpio.h"
void BUZZER_Init()
{
	GPIO_setupPinDirection(PORTC_ID, PIN2_ID, PIN_OUTPUT);
	GPIO_writePin(PORTC_ID, PIN2_ID, LOGIC_LOW);
}
void BUZZER_On()
{
	GPIO_writePin(PORTC_ID, PIN2_ID, LOGIC_HIGH);
}
void BUZZER_Off()
{
	GPIO_writePin(PORTC_ID, PIN2_ID, LOGIC_LOW);
}
