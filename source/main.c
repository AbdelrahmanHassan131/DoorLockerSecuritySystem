#include "app.h"

int main()
{
	App_Init();
	ProtectionState DoorProtectionState = NO_PASSWORD;

	while (1)
	{
		if (DoorProtectionState == NO_PASSWORD)
		{
			App_CreatePassword(DoorProtectionState);
		}
		else
		{
			App_WelcomeScreen();
		}
	}

	return 0;
}
