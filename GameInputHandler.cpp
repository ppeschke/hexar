#include "GameInputHandler.h"
#include "game.h"
#include "NetworkClient.h"

NetworkClient* getClient();

GameInputHandler::GameInputHandler(Game* g, NetworkClient* c) : InputHandler(g, c)
{
}


GameInputHandler::~GameInputHandler()
{
}

void GameInputHandler::handleMouseClick(InputEvent e)
{
}

void GameInputHandler::handleButtonPress(InputEvent e)
{
	if(thegame->typing)
	{
		switch(e.getPressed())
		{
		case (char)27:	//escape
				thegame->typing = false;
			break;
		case (char)9:	//tab
			thegame->typing = false;
			break;
		case (char)8:	//backspace
			if(thegame->chatString.size() > 0)
				thegame->chatString = thegame->chatString.substr(0, thegame->chatString.size() - 1);
			break;
		case (char)10:	//line feed
		case (char)13:	//carriage return
			getClient()->Send(thegame->chatString.c_str());
			thegame->chatString = "";
			thegame->typing = false;
			break;
		default:
			if(thegame->typing)
				thegame->chatString += e.getPressed();
			break;
		}
	}
	else
	{
		switch(e.getPressed())
		{
		case 't':
		case 'T':
			break;
		case 'w':
		case 'W':
			break;
		case 'b':
		case 'B':
			break;
		case 'm':
		case 'M':
			break;
		case 'g':
		case 'G':
			break;
		case ' ':
			break;
		default:
			break;
		}
	}
}
