/*
 * dcmotor.c
 *
 *  Created on: Apr 8, 2023
 *      Author: Abdelrahman
 */


#include "dcmotor.h"
#include "gpio.h"
#include "pwm.h"

void DCMotor_Init(void)
{
	GPIO_setupPinDirection(PORTB_ID, PIN0_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(PORTB_ID, PIN1_ID, PIN_OUTPUT);
	GPIO_setupPinDirection(PORTB_ID, PIN3_ID, PIN_OUTPUT);

	GPIO_writePin(PORTB_ID, PIN0_ID, LOGIC_LOW);
	GPIO_writePin(PORTB_ID, PIN1_ID, LOGIC_LOW);
	GPIO_writePin(PORTB_ID, PIN3_ID, LOGIC_LOW);
}
void DCMotor_Rotate(DCMotor_State dcMotorState, uint8 speed)
{
	switch (dcMotorState)
	{
	case DC_Motor_State_STOP:
		GPIO_writePin(PORTB_ID, PIN0_ID, LOGIC_LOW);
		GPIO_writePin(PORTB_ID, PIN1_ID, LOGIC_LOW);
		break;

	case DC_Motor_State_ACW:
		GPIO_writePin(PORTB_ID, PIN0_ID, LOGIC_HIGH);
		GPIO_writePin(PORTB_ID, PIN1_ID, LOGIC_LOW);
		break;
	case DC_Motor_State_CW:
		GPIO_writePin(PORTB_ID, PIN0_ID, LOGIC_LOW);
		GPIO_writePin(PORTB_ID, PIN1_ID, LOGIC_HIGH);
		break;
	}

	PWM_TIMER0_Start(speed);

}
