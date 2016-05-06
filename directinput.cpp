#include "global.h"

LPDIRECTINPUT8 din;
LPDIRECTINPUTDEVICE8 dinkeyboard;
LPDIRECTINPUTDEVICE8 dinmouse;

// Create DirectInput and the Keyboard and Mouse Devices
void InitDirectInput(HINSTANCE hInstance, GAMEWINDOW* gw)
{
    DirectInput8Create(hInstance,
                      DIRECTINPUT_VERSION,
                      IID_IDirectInput8,
                      (void**)&din,
                      NULL);

    din->CreateDevice(GUID_SysKeyboard, &dinkeyboard, NULL);
    din->CreateDevice(GUID_SysMouse, &dinmouse, NULL);

    dinkeyboard->SetDataFormat(&c_dfDIKeyboard);
    dinmouse->SetDataFormat(&c_dfDIMouse);

    dinkeyboard->SetCooperativeLevel(gw->hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
    dinmouse->SetCooperativeLevel(gw->hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

    dinmouse->Acquire();

    return;
}

// Get the Current Keyboard Status
void GetKeys(BYTE* KeyState)
{
    dinkeyboard->Acquire();

    dinkeyboard->GetDeviceState(256, (LPVOID)KeyState);

    for(int Index = 0; Index < 256; Index++)
        *(KeyState + Index) &= 0x80;

    return;
}

// Get the Latest Mouse Movements and Mouse Buttons
void GetMouse(DIMOUSESTATE* MouseState)
{
    dinmouse->Acquire();

    dinmouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)MouseState);

    MouseState->rgbButtons[0] &= 0x80;
    MouseState->rgbButtons[1] &= 0x80;
    MouseState->rgbButtons[2] &= 0x80;
    MouseState->rgbButtons[3] &= 0x80;

    return;
}

// Close the Devices and DirectInput
void CloseDirectInput(void)
{
    dinkeyboard->Unacquire();
    dinmouse->Unacquire();
    din->Release();

    return;
}