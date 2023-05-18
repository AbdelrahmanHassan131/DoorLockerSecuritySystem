#include "app.h"

int main()
{
	App_Init();
	ProtectionState DoorProtectionState = NO_PASSWORD;

	while (1)
	{
		switch(DoorProtectionState)
		{
			case NO_PASSWORD:
				App_CreatePassword(DoorProtectionState);
				break;
			case PASSWORD_CREATED:
				App_SendPassword(DoorProtectionState);
				break;
			case PASSWORD_PROTECTED:
				//App_WelcomeScreen();
				break;

		}
	}

	return 0;
}
