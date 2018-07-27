#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "inputdata.h"

void GetKeys(BYTE* KeyState);
void GetMouse(DIMOUSESTATE* MouseState);

void Input(INPUTDATA* InputData)
{
    static DIMOUSESTATE Mouse;
	static bool ClickStarted = false;
	static bool Clicked = false;

	if(InputData->Clicked == false)	//input was handled
		Clicked = false;

    GetMouse(&Mouse);

    ZeroMemory(InputData, sizeof(INPUTDATA));

    InputData->MouseX = (float)Mouse.lX;
    InputData->MouseY = (float)Mouse.lY;
    InputData->MouseZ = (float)Mouse.lZ;

	if(Mouse.rgbButtons[0])
	{
		InputData->MouseButton = true;
		ClickStarted = true;
	}
	else
	{
		if(ClickStarted)
		{
			Clicked = true;
			ClickStarted = false;
		}
	}
	InputData->Clicked = Clicked;
    return;
}
