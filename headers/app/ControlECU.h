/*
 * ControlECU.h
 *
 *  Created on: May 19, 2023
 *      Author: Abdelrahman
 */

#ifndef HEADERS_APP_CONTROLECU_H_
#define HEADERS_APP_CONTROLECU_H_

#include "std_types.h"

uint8 CECU_Password[8];
PasswordStage m_PasswordStage;
uint8 CECU_SuccessiveErrorPassword;

void ControlECU_Init(ProtectionState *doorProtectionState);
void ControlECU_ReceivePassword();
uint8 ControlECU_SavePassword();
ProtectionState ControlECU_CheckForSavedPasswords();
void ControlECU_CreatePassword(ProtectionState *doorProtectionState);
uint8 ControlECU_IsPasswordMatch();
uint8 ControlECU_CheckPasswordReceived();
uint8 ControlECU_ComparePasswords(uint8 * firstPassword, uint8 * secondPassword, uint8 size);
uint8 ControlECU_ReceiveControl();
void ControlECU_OpenDoor();
void ControlECU_GetSavedPassword(uint8 *passwordSaved);
void ControlECU_ChangePassword(ProtectionState *doorProtectionState);
void ControlECU_SecurityErrorPassword();


#endif /* HEADERS_APP_CONTROLECU_H_ */
