#include "std_types.h"

#if ECU
#include "UIECU.h"
int main()
{
	ProtectionState doorProtectionState;
	UIECU_Init(&doorProtectionState);

	while (1)
	{
		switch(doorProtectionState)
		{
			case NO_PASSWORD:
				UIECU_CreatePassword(&doorProtectionState);
				break;
			case PASSWORD_PROTECTED:
				UIECU_MainScreen(&doorProtectionState);
				break;
		}
	}

	return 0;
}
#else
#include "lcd.h"
#include "ControlECU.h"
int main()
{
	ProtectionState doorProtectionState;
	ControlECU_Init(&doorProtectionState);

	while (1)
	{
		switch(doorProtectionState)
		{
			case NO_PASSWORD:
				ControlECU_CreatePassword(&doorProtectionState);
				break;
			case PASSWORD_PROTECTED:

				if (ControlECU_CheckPasswordReceived())
				{
					uint8 controlReceived = ControlECU_ReceiveControl();
					if (controlReceived == '+')
					{
						ControlECU_OpenDoor();
					}
					else if (controlReceived == '-')
					{
						ControlECU_ChangePassword(&doorProtectionState);
					}
				}
				else
				{
					ControlECU_SecurityErrorPassword();
				}

				break;
		}
	}

	return 0;
}
#endif
