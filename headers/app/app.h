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
uint8 passwordLenght = 0;

void App_Init();
void App_CreatePassword(ProtectionState doorProtectionState);
void App_WritePassword(uint8 password);
void App_SavePassword();

#endif /* HEADERS_APP_APP_H_ */
