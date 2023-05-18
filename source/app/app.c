#include "app.h"
#include "lcd.h"
#include "uart.h"
#include "keypad.h"
#include "avr/io.h"
#include "util/delay.h"

void App_Init()
{
	SREG |= (1<<7);
	LCD_init();
	UART_init(9600);
	passwordStage = ENTERING_PASSWORD_STATE;
	m_KeyPressed = 11;
}

void App_CreatePassword(ProtectionState doorProtectionState)
{
	switch(passwordStage)
	{
	case ENTERING_PASSWORD_STATE:
		LCD_displayString("plz enter pass: ");
		LCD_moveCursor(1, 0);
		passwordStage = WRITEING_PASSWORD_STATE;
		break;
	case WRITEING_PASSWORD_STATE:
		App_WritePassword(m_KeyPressed);
		if (passwordLenght == 4)
		{
			passwordStage = REENTERING_PASSWORD_STATE;
		}
		break;
	case REENTERING_PASSWORD_STATE:
		LCD_moveCursor(0, 0);
		LCD_displayString("plz re-enter the");
		LCD_moveCursor(1, 0);
		LCD_displayString("same pass: ");
		passwordLenght = 0;
		passwordStage = CONFIRMING_PASSWORD_STATE;
		break;
	case CONFIRMING_PASSWORD_STATE:
		if(!App_ConfirmPassword(m_KeyPressed))
		{
			passwordStage = ENTERING_PASSWORD_STATE;
			passwordLenght = 0;
		}
		if (passwordLenght == 4)
		{
			passwordStage = SAVED_PASSWORD_STATE;

		}
		break;
	case SAVED_PASSWORD_STATE:
		//App_SavePassword();
		doorProtectionState = PASSWORD_PROTECTED;
		break;

	}

	m_KeyPressed = KEYPAD_getPressedKey();
	_delay_ms(500);
}

void App_WritePassword(uint8 passwordChar)
{
	if (passwordChar >=0 && passwordChar <=9 && passwordLenght < 4)
	{
		m_Password[passwordLenght] = passwordChar;
		passwordLenght++;
		LCD_displayCharacter('*');
	}
}

void App_SendPassword(ProtectionState doorProtectionState)
{
	UART_sendString(m_Password);
}

uint8 App_ConfirmPassword(uint8 passwordChar)
{
	if (passwordChar >=0 && passwordChar <=9 && passwordLenght < 4)
	{
		if (m_Password[passwordLenght] == passwordChar)
		{
			LCD_displayCharacter('*');
			passwordLenght++;
			return 1;
		}
		else
		{
			return 0;
		}
	}
}
