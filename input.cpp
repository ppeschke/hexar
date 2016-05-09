#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#include "inputdata.h"

void GetKeys(BYTE* KeyState);
void GetMouse(DIMOUSESTATE* MouseState);

void Input(INPUTDATA* InputData)
{
    static BYTE Keys[256];
    static DIMOUSESTATE Mouse;

    GetKeys(&Keys[0]);
    GetMouse(&Mouse);

    ZeroMemory(InputData, sizeof(INPUTDATA));

    if(Keys[DIK_LEFT] || Keys[DIK_A])
        InputData->MoveLeft = true;
    if(Keys[DIK_RIGHT] || Keys[DIK_D])
        InputData->MoveRight = true;
    if(Keys[DIK_UP] || Keys[DIK_W])
        InputData->MoveUp = true;
    if(Keys[DIK_DOWN] || Keys[DIK_S])
        InputData->MoveDown = true;
	if(Keys[DIK_ESCAPE])
		InputData->Esc = true;
	if(Keys[DIK_SPACE])
		InputData->EndTurn = true;
	else if(Keys[DIK_G])
		InputData->G = true;
	else if(Keys[DIK_M])
		InputData->M = true;
	else if(Keys[DIK_T])
		InputData->T = true;
	else if(Keys[DIK_W])
		InputData->W = true;
	else if(Keys[DIK_B])
		InputData->B = true;

	

    InputData->MouseX = (float)Mouse.lX;
    InputData->MouseY = (float)Mouse.lY;
    InputData->MouseZ = (float)Mouse.lZ;

	if(Mouse.rgbButtons[0])
		InputData->MouseButton = true;
    return;
} 