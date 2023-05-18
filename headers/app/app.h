/*
 * app.h
 *
 *  Created on: Apr 7, 2023
 *      Author: Abdelrahman
 */

#ifndef HEADERS_APP_APP_H_
#define HEADERS_APP_APP_H_

#include "std_types.h"

PasswordStage passwordStage;
char m_Password[4];
uint8 passwordLenght;
uint8 m_KeyPressed;

void App_Init();
void App_CreatePassword(ProtectionState doorProtectionState);
void App_SendPassword(ProtectionState doorProtectionState);
void App_WritePassword(uint8 password);
void App_SavePassword();
uint8 App_ConfirmPassword(uint8 keyPressed);

#endif /* HEADERS_APP_APP_H_ */
