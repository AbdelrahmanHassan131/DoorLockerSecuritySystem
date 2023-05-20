#include "lcd.h"
#include "uart.h"
#include "ControlECU.h"
#include "external_eeprom.h"
#include "twi.h"
#include "util/delay.h"
#include "avr/io.h"
#include "buzzer.h"
#include "timer1.h"

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
	CECU_tick = 0;
	CECU_continue = 0;
	CECU_waitSeconds = 0;
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
	CECU_SuccessiveErrorPassword = 0;
	UART_sendByte(PASSWORD_PROTECTED);
	UART_sendByte(DOOR_LOCK_DOOR_OPENING_STATE);
	DCMotor_Rotate(DC_Motor_State_CW, 100);
	CECU_WaitSeconds(15);
	UART_sendByte(DOOR_LOCK_DOOR_CLOSING);
	DCMotor_Rotate(DC_Motor_State_STOP, 0);
	CECU_WaitSeconds(3);
	DCMotor_Rotate(DC_Motor_State_ACW, 100);
	CECU_WaitSeconds(15);
	DCMotor_Rotate(DC_Motor_State_STOP, 0);
	UART_sendByte(DOOR_LOCK_MAIN_MENU_STATE);
}

void ControlECU_ChangePassword(ProtectionState *doorProtectionState)
{
	CECU_SuccessiveErrorPassword = 0;
	UART_sendByte(NO_PASSWORD);
	UART_sendByte(DOOR_LOCK_MAIN_MENU_STATE);
	*doorProtectionState = NO_PASSWORD;
	m_PasswordStage = ENTERING_PASSWORD_STATE;
}

uint8 ControlECU_ReceiveControl()
{
	UART_sendByte(PASSWORD_STATE_ACCEPTED);
	return UART_recieveByte();
}

void ControlECU_SecurityErrorPassword()
{
	CECU_SuccessiveErrorPassword++;
	if (CECU_SuccessiveErrorPassword == 3)
	{

		BUZZER_On();
		UART_sendByte(PASSWORD_STATE_BUZZER);
		CECU_WaitSeconds(60);
		UART_sendByte(1);
		BUZZER_Off();
		CECU_SuccessiveErrorPassword = 0;
	}
	else
	{
		UART_sendByte(PASSWORD_STATE_REJECTED);
	}
	//the stages of buzzer and counter of how many entered wrong password
}

void ControlECU_CallBackTimer1_tickCounter()
{
	CECU_tick++;
	if (CECU_tick == CECU_waitSeconds)
	{
		CECU_tick = 0;
		CECU_continue = 1;
	}
}

void CECU_WaitSeconds(uint8 timeToWait)
{
	CECU_waitSeconds = timeToWait;
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "ERROR WAIT 1");
	LCD_moveCursor(1, 0);
	Timer1_ConfigType configType = {0,0x1E85,TIMER1_PRESCALER_1024,TIMER1_MODE_COMPARE};
	Timer1_Init(&configType);
	Timer1_SetCallBack(ControlECU_CallBackTimer1_tickCounter);
	while (!CECU_continue);
	CECU_continue = 0;
	Timer1_DeInit();
}
