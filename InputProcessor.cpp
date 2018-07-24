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
	GetKeys(&Keys[0]);
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
	for(int k = 0; k <= 0x52; ++k)
	{
		if(!Keys[k] && prevKeys[k])	//if key was pressed last frame, but let up this frame,
		{
			events.push_back(k);
		}
	}
	ProcessEvents();
	//swap the pointers
	BYTE* temp = Keys;
	Keys = prevKeys;
	prevKeys = temp;
}

void InputProcessor::ProcessEvents()
{
	bool shift = Keys[DIK_LSHIFT] || Keys[DIK_RSHIFT];
	static bool caps = false;
	if(thegame->arrow != nullptr)
	{
		thegame->arrow->x -= x / 20.0f;
		thegame->arrow->z += y / 20.0f;
	}
	if(Clicked)
	{
		//handled
		Clicked = false;
	}
	while(!events.empty())
	{
		short code = (*events.begin());
		events.pop_front();
		switch(code)
		{
		case DIK_ESCAPE:
			thegame->over = true;
			break;
		case DIK_BACK:
			if(thegame->typing)
				if(thegame->chatString != "")
					thegame->chatString = thegame->chatString.substr(0, thegame->chatString.size() - 1);
			break;
		case DIK_TAB:
			thegame->typing = !thegame->typing;
			break;
		case DIK_RETURN:
		case DIK_NUMPADENTER:
			if(thegame->typing)
			{
				thegame->messages.AddMessage(thegame->chatString, 1.0f);
				thegame->chatString = "";
				thegame->typing = false;
			}
			break;
		case DIK_LSHIFT://we don't need to process shift key events, just skip them if they occur
		case DIK_RSHIFT://we're only interested in instantaneous shift key presses
			continue;
			break;
		case DIK_CAPITAL:
			caps = !caps;
			break;
		case DIK_DIVIDE:
			break;
		default:
			char c = characterMap[(shift? code + 0x52:code)];
			if(thegame->typing)
			{
				if(caps && isalpha(c))
					c = toupper(c);
				thegame->chatString += c;
			}
			else
			{
				switch(c)
				{
				case 'g':
					break;
				case 'b':
					break;
				case 'w':
					break;
				case 't':
					break;
				default:
					break;
				}
			}
			break;
		}
	}
}
