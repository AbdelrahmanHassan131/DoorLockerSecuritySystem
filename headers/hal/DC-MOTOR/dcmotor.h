/*
 * dcmotor.h
 *
 *  Created on: Apr 8, 2023
 *      Author: Abdelrahman
 */

#ifndef HEADERS_HAL_DC_MOTOR_DCMOTOR_H_
#define HEADERS_HAL_DC_MOTOR_DCMOTOR_H_

#include "std_types.h"


void DCMotor_Init(void);
void DCMotor_Rotate(DCMotor_State dcMotorState, uint8 speed);



#endif /* HEADERS_HAL_DC_MOTOR_DCMOTOR_H_ */
