#include "InputProcessor.h"

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

//maps DirectInput codes to actual characters -- add 0x52 when shift is pressed
char characterMap[] = { ' ', ' ', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', ' ', '\t',		//0f
'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', ' ', ' ', 'a', 's',		//1f
'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', ' ', '\\', 'z', 'x', 'c', 'v',	//2f
'b', 'n', 'm', ',', '.', '/', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',		//3f
' ', ' ', ' ', ' ', ' ', ' ', ' ', '7', '8', '9', '-', '4', '5', '6', '+', '1',		//4f
'2', '3', '0', '.', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '-', '=',		//5f
' ', ' ', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', ' ', ' ',		//6f
'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', ' ', '|', 'Z', 'X',		//7f
'C', 'V', 'B', 'N', 'M', '<', '>', '?', ' ', '*', ' ', ' ', ' ', ' ', ' ', ' ',		//8f
' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '7', '8', '9', '-', '4', '5', '6',		//9f
'+', '1', '2', '3', '0', '.', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '		//AF
};

void GetMouse(DIMOUSESTATE* MouseState);
void GetKeys(BYTE*);

InputProcessor::InputProcessor(Game* tg)
{
	Keys = new BYTE[256];
	prevKeys = new BYTE[256];
	GetKeys(prevKeys);
	thegame = tg;
	Clicked = false;
}

InputProcessor::~InputProcessor()
{
	delete[] Keys;
	delete[] prevKeys;
}

void InputProcessor::ProcessInputs()
{
	static DIMOUSESTATE Mouse;
	static bool ClickStarted = false;
	//mouse
	GetMouse(&Mouse);
	x = (float)Mouse.lX;
	y = (float)Mouse.lY;
	z = (float)Mouse.lZ;

	if(Mouse.rgbButtons[0])
		ClickStarted = true;
	if(!Mouse.rgbButtons[0] && ClickStarted)
	{
		Clicked = true;
		ClickStarted = false;
	}
	events.push_back(InputEvent(mouse, '!', x / 20.0f, y / 20.0f));
	if(Clicked)
	{
		events.push_back(InputEvent(mouse, 'c', 0.0f, 0.0f));
		//handled
		Clicked = false;
	}

	//keyboard
	GetKeys(&Keys[0]);
	if(prevKeys[DIK_CAPITAL] != Keys[DIK_CAPITAL])	//if state is different from last frame
		caps = !caps;								//toggle state
	for(int k = 0; k <= 0x52; ++k)
	{
		if(prevKeys[k] && !Keys[k])	//if key was pressed last frame, but let up this frame,
		{
			bool shift = Keys[DIK_LSHIFT] || Keys[DIK_RSHIFT];
			if(!(k == DIK_LSHIFT || k == DIK_RSHIFT || k == DIK_CAPITAL))	//we don't need to process shift key events, just skip them if they occur
				events.push_back(TranslateKeyboardEvent(k, shift));
		}
	}
	
	//swap the pointers
	BYTE* temp = Keys;
	Keys = prevKeys;
	prevKeys = temp;
}

InputEvent InputProcessor::TranslateKeyboardEvent(short code, bool shift)
{
	switch(code)
	{
	case DIK_ESCAPE:
		return InputEvent(keyboard, (char)27, 0.0f, 0.0f);
		break;
	case DIK_BACK:
		return InputEvent(keyboard, (char)8, 0.0f, 0.0f);
		break;
	case DIK_TAB:
		return InputEvent(keyboard, (char)9, 0.0f, 0.0f);
		break;
	case DIK_RETURN:
	case DIK_NUMPADENTER:
		return InputEvent(keyboard, (char)10, 0.0f, 0.0f);
		break;
	case DIK_DIVIDE:
		return InputEvent(keyboard, '/', 0.0f, 0.0f);
		break;
	default:
		char c = characterMap[(shift? code + 0x52:code)];
		return InputEvent(keyboard, c, 0.0f, 0.0f);
		break;
	}
}

list<InputEvent>* InputProcessor::getEvents()
{
	return &this->events;
}