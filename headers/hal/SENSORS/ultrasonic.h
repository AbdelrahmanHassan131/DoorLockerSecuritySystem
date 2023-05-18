/*
 * ultrasonic.h
 *
 *  Created on: Apr 28, 2023
 *      Author: Abdelrahman
 */

#ifndef HEADERS_HAL_ULTRASONIC_ULTRASONIC_H_
#define HEADERS_HAL_ULTRASONIC_ULTRASONIC_H_

#include "std_types.h"

void Ultrasonic_Init();
void Ultrasonic_Trigger();
uint16 Ultrasonic_readDistance();
void Ultrasonic_edgeProcessing();
uint16 Ultrasonic_Calc_Distance();

#endif /* HEADERS_HAL_ULTRASONIC_ULTRASONIC_H_ */
