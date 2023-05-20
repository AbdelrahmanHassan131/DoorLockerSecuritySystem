/*
 * app.h
 *
 *  Created on: Apr 7, 2023
 *      Author: Abdelrahman
 */

#ifndef HEADERS_APP_UIECU_H_
#define HEADERS_APP_UIECU_H_

#include "std_types.h"

PasswordStage UIECU_PasswordStage;
char UIECU_Password[7];
uint8 UIECU_PasswordLenght;
uint8 UIECU_KeyPressed;
DoorSecurityState UIECU_DoorSecuritystates;
uint8 UIECU_Control;
DoorState UIECU_DoorState;

void UIECU_Init(ProtectionState *doorProtectionState);
void UIECU_CreatePassword(ProtectionState *doorProtectionState);
void UIECU_SendPassword();
void UIECU_WritePassword(uint8 password);
uint8 UIECU_ConfirmPassword(uint8 keyPressed);
void UIECU_MainScreen(ProtectionState *doorProtectionState);
ProtectionState UIECU_CheckForSavedPasswords();
uint8 UIECU_IsPasswordMatching(uint8 *password);
DoorState UIECU_GetDoorState();
ProtectionState UIECU_GetDoorSecuritystates();

#endif /* HEADERS_APP_UIECU_H_ */
