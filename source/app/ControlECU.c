#include "lcd.h"
#include "uart.h"
#include "ControlECU.h"
#include "external_eeprom.h"
#include "twi.h"
#include "util/delay.h"
#include "avr/io.h"
#include "buzzer.h"

void ControlECU_Init(ProtectionState *doorProtectionState)
{
	SREG |= (1<<7);
	LCD_init();
	DCMotor_Init();
	UART_init(9600);
	TWI_ConfigType twiConfigType = {0x00,400000};
	TWI_init(&twiConfigType);
	m_PasswordStage = ENTERING_PASSWORD_STATE;
	*doorProtectionState = NO_PASSWORD;
	CECU_SuccessiveErrorPassword = 0;
}

void ControlECU_ReceivePassword()
{
	UART_receiveString(CECU_Password);
}

uint8 ControlECU_SavePassword()
{
	uint8 * p = 0x00;
	for (uint8 i = 0 ; i < 5; i++)
	{
		if (EEPROM_writeByte(p++ , CECU_Password[i]) == ERROR)
		{
			return 0;
		}
		_delay_ms(10);
	}
	return 1;
}
ProtectionState ControlECU_CheckForSavedPasswords()
{
	//goto eeprom check if there is a password saved
	EEPROM_writeByte(0x0A, 0);
	return NO_PASSWORD;
}

void ControlECU_CreatePassword(ProtectionState *doorProtectionState)
{
	switch(m_PasswordStage)
	{
		case ENTERING_PASSWORD_STATE:
			ControlECU_ReceivePassword();
			LCD_displayString("Received Password");
			m_PasswordStage = CONFIRMING_PASSWORD_STATE;
			break;
		case CONFIRMING_PASSWORD_STATE:
			if (ControlECU_IsPasswordMatch() && ControlECU_SavePassword())
			{
				*doorProtectionState = PASSWORD_PROTECTED;
				UART_sendByte(1);
			}
			else
			{
				m_PasswordStage = ENTERING_PASSWORD_STATE;
				UART_sendByte(0);
			}
			break;
	}
}

uint8 ControlECU_IsPasswordMatch()
{
	uint8 repeatedPassword[7];
	UART_receiveString(repeatedPassword);
	return ControlECU_ComparePasswords(repeatedPassword, CECU_Password, 5);
}

uint8 ControlECU_ComparePasswords(uint8 * firstPassword, uint8 * secondPassword, uint8 size)
{
	for (uint8 i= 0 ; i < size; i++)
	{
		if (firstPassword[i] != secondPassword[i])
		{
			return 0;
		}

	}
	return 1;
}

uint8 ControlECU_CheckPasswordReceived()
{
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Wait Check Pass");
	uint8 receivedPassword[7];
	UART_receiveString(receivedPassword);
	uint8 savedPassword[7];
	ControlECU_GetSavedPassword(savedPassword);
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "got Saved Pass ");
	uint8 isPasswordCorrect =  ControlECU_ComparePasswords(receivedPassword, savedPassword, 5);
	UART_sendByte(isPasswordCorrect);
	return isPasswordCorrect;
}


void ControlECU_GetSavedPassword(uint8 *passwordSaved)
{
	for (uint8 i = 0 ; i < 5; i++)
	{
		LCD_intgerToString(EEPROM_readByte(0x00 + i, &passwordSaved[i]));
		_delay_ms(10);
	}
}

void ControlECU_OpenDoor()
{
	UART_sendByte(PASSWORD_PROTECTED);
	UART_sendByte(DOOR_LOCK_DOOR_OPENING_STATE);
	DCMotor_Rotate(DC_Motor_State_CW, 100);
	_delay_ms(3000);
	UART_sendByte(DOOR_LOCK_DOOR_CLOSING);
	DCMotor_Rotate(DC_Motor_State_STOP, 0);
	_delay_ms(3000);
	DCMotor_Rotate(DC_Motor_State_ACW, 100);
	_delay_ms(3000);
	DCMotor_Rotate(DC_Motor_State_STOP, 0);
	UART_sendByte(DOOR_LOCK_MAIN_MENU_STATE);
}

void ControlECU_ChangePassword(ProtectionState *doorProtectionState)
{
	UART_sendByte(NO_PASSWORD);
	UART_sendByte(DOOR_LOCK_MAIN_MENU_STATE);
	*doorProtectionState = NO_PASSWORD;
	m_PasswordStage = ENTERING_PASSWORD_STATE;
}

uint8 ControlECU_ReceiveControl()
{
	return UART_recieveByte();
}

void ControlECU_SecurityErrorPassword()
{
	CECU_SuccessiveErrorPassword++;
	if (CECU_SuccessiveErrorPassword == 3)
	{
		BUZZER_On();
		//polling so nothing be accepted
		while(1);
		_delay_ms(5000);
	}
	//the stages of buzzer and counter of how many entered wrong password
}
