#include "UIECU.h"
#include "lcd.h"
#include "uart.h"
#include "keypad.h"
#include "avr/io.h"
#include "util/delay.h"

void UIECU_Init(ProtectionState *doorProtectionState)
{
	SREG |= (1<<7);
	LCD_init();
	UIECU_PasswordStage = ENTERING_PASSWORD_STATE;
	UART_init(9600);
	UIECU_KeyPressed = 11;
	*doorProtectionState = NO_PASSWORD;
	UIECU_DoorSecuritystates = DOOR_LOCK_MAIN_MENU_STATE;
}

void UIECU_CreatePassword(ProtectionState *doorProtectionState)
{
	switch(UIECU_PasswordStage)
	{
	case ENTERING_PASSWORD_STATE:
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "plz enter pass: ");
		UIECU_PasswordLenght = 0;
		LCD_moveCursor(1, 0);
		UIECU_PasswordStage = WRITEING_PASSWORD_STATE;
		break;
	case WRITEING_PASSWORD_STATE:
		UIECU_KeyPressed = KEYPAD_getPressedKey();
		_delay_ms(250);
		if (UIECU_PasswordLenght == 5 && UIECU_KeyPressed == '=')
		{
			UIECU_SendPassword();
			UIECU_PasswordStage = REENTERING_PASSWORD_STATE;
			UIECU_KeyPressed = 11;
		}
		else
		{
			UIECU_WritePassword(UIECU_KeyPressed);
		}

		break;
	case REENTERING_PASSWORD_STATE:
		LCD_clearScreen();
		LCD_displayString("plz re-enter the");
		LCD_moveCursor(1, 0);
		LCD_displayString("same pass: ");
		UIECU_PasswordLenght = 0;
		UIECU_PasswordStage = CONFIRMING_PASSWORD_STATE;
		break;
	case CONFIRMING_PASSWORD_STATE:
		UIECU_KeyPressed = KEYPAD_getPressedKey();
		_delay_ms(250);
		if (UIECU_KeyPressed == '=' && UIECU_PasswordLenght == 5)
		{
			PasswordState passwordState = UIECU_IsPasswordMatching(UIECU_Password);
			if (passwordState == PASSWORD_STATE_ACCEPTED)
			{
				UIECU_PasswordStage = ENTERING_PASSWORD_STATE;
				*doorProtectionState = PASSWORD_PROTECTED;
				LCD_clearScreen();
				LCD_moveCursor(0, 0);
				UIECU_PasswordLenght = 0;
				UIECU_KeyPressed = 11;
				UIECU_DoorSecuritystates = DOOR_LOCK_MAIN_MENU_STATE;
			}
			else if (passwordState == PASSWORD_STATE_REJECTED)
			{
				UIECU_PasswordStage = ENTERING_PASSWORD_STATE;
				LCD_clearScreen();
				LCD_moveCursor(0, 0);
				UIECU_PasswordLenght = 0;
			}
			else if (passwordState == PASSWORD_STATE_BUZZER)
			{
				LCD_clearScreen();
				LCD_displayStringRowColumn(0, 0, "Error");
				UART_recieveByte();
			}
		}
		else
		{
			UIECU_WritePassword(UIECU_KeyPressed);
		}
		break;


	}
}

void UIECU_WritePassword(uint8 passwordChar)
{
	if (passwordChar >=0 && passwordChar <=9 && UIECU_PasswordLenght < 5)
	{
		UIECU_Password[UIECU_PasswordLenght] = passwordChar + '0';
		UIECU_PasswordLenght++;
		LCD_displayCharacter('*');
	}
}

void UIECU_SendPassword()
{
	UIECU_Password[5] = '#';
	UIECU_Password[6] = '\0';
	UART_sendString(UIECU_Password);
}

void UIECU_MainScreen(ProtectionState *doorProtectionState)
{
	switch(UIECU_DoorSecuritystates)
	{
		case DOOR_LOCK_MAIN_MENU_STATE:
			LCD_displayStringRowColumn(0, 0, "+ : Open Door");
			LCD_displayStringRowColumn(1, 0, "- : Change Pass");
			UIECU_Control = KEYPAD_getPressedKey();
			_delay_ms(250);
			if (UIECU_Control == '+' || UIECU_Control == '-')
			{
				LCD_displayCharacter('+');
				UIECU_DoorSecuritystates = DOOR_LOCK_ENTER_PASSWORD_STATE;
				UIECU_KeyPressed = 11;
			}
			break;
		case DOOR_LOCK_ENTER_PASSWORD_STATE:
			if (UIECU_KeyPressed == 11)
			{
				LCD_clearScreen();
				LCD_displayString("plz enter pass: ");
				LCD_moveCursor(1, 0);
			}
			UIECU_KeyPressed = KEYPAD_getPressedKey();
			_delay_ms(250);
			if (UIECU_KeyPressed == '=' && UIECU_PasswordLenght == 5)
			{
				PasswordState passwordState = UIECU_IsPasswordMatching(UIECU_Password);
				if (passwordState == PASSWORD_STATE_ACCEPTED)
				{
					UART_sendByte(UIECU_Control);
					*doorProtectionState = UIECU_GetDoorSecuritystates();
					UIECU_DoorSecuritystates = 	UIECU_GetDoorState();


					UIECU_KeyPressed = 11;
				}
				else if (passwordState == PASSWORD_STATE_BUZZER)
				{
					LCD_clearScreen();
					LCD_displayStringRowColumn(0, 0, "Error 3 Times");
					UART_recieveByte();
					UIECU_PasswordLenght = 0;
					UIECU_DoorSecuritystates = DOOR_LOCK_MAIN_MENU_STATE;
				}
				else
				{
					UIECU_PasswordLenght = 0;
					UIECU_DoorSecuritystates = DOOR_LOCK_MAIN_MENU_STATE;
				}
			}
			else
			{
				UIECU_WritePassword(UIECU_KeyPressed);
			}
			break;
		case DOOR_LOCK_DOOR_OPENING_STATE:
			LCD_clearScreen();
			LCD_displayStringRowColumn(0, 0, "Door Is Unlocking");
			UIECU_DoorSecuritystates = UIECU_GetDoorState();
			break;
		case DOOR_LOCK_DOOR_CLOSING:
			LCD_clearScreen();
			LCD_displayStringRowColumn(0, 0, "Door Is Locking");
			UIECU_DoorSecuritystates = UIECU_GetDoorSecuritystates();
			UIECU_PasswordLenght = 0;
			UIECU_KeyPressed = 11;
			break;
	}
}


DoorState UIECU_GetDoorState()
{
	return UART_recieveByte();
}

ProtectionState UIECU_CheckForSavedPasswords()
{
	return UART_recieveByte();
}

uint8 UIECU_IsPasswordMatching(uint8 *password)
{
	UIECU_SendPassword(password);
	return UART_recieveByte();
}


ProtectionState UIECU_GetDoorSecuritystates()
{
	return UART_recieveByte();
}
