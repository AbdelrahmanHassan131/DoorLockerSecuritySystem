#include "app.h"
#include "lcd.h"
#include "keypad.h"
#include "avr/io.h"

void App_Init()
{
	SREG |= (1<<7);
	LCD_init();
	passwordStage = ENTERING_PASSWORD_STATE;
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
		uint8 keyPressed = KEYPAD_getPressedKey();
		App_WritePassword(keyPressed);
		LCD_displayCharacter("*");
		if (passwordLenght == 4)
		{
			passwordStage = CONFIRMING_PASSWORD_STATE;
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
		uint8 keyPressed = KEYPAD_getPressedKey();
		if(!App_ConfirmPassword(keyPressed))
		{
			passwordStage = ENTERING_PASSWORD_STATE;
			passwordLenght = 0;
		}
		LCD_displayCharacter("*");
		if (passwordLenght == 4)
		{
			passwordStage = SAVED_PASSWORD_STATE;

		}
		break;
	case SAVED_PASSWORD_STATE:
		App_SavePassword();
		doorProtectionState = PASSWORD_PROTECTED;
		break;

	}
}

void App_WritePassword(uint8 passwordChar)
{
	if (passwordChar >=0 && passwordChar <=9 && passwordLenght < 4)
	{
		m_Password[passwordLenght] = passwordChar;
		passwordLenght++;
	}
}
