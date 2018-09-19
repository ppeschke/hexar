#include "GameInputHandler.h"
#include "game.h"
#include "NetworkClient.h"

NetworkClient* getClient();
bool getHovered(Game* thegame, int& i, int& p);
base* getHexagon(Game* thegame, int i, int p);
base* getItem(Game* thegame, int i, int p);
string toString(int i);

GameInputHandler::GameInputHandler(Game* g, NetworkClient* c) : InputHandler(g, c)
{
}


GameInputHandler::~GameInputHandler()
{
}

void GameInputHandler::handleMouseClick(InputEvent e)
{
	int i = -1, p = -1;
	switch(thegame->command)
	{
	case 'B':
		if(thegame->sel1 == nullptr)
		{
			//may set i and p
			getHovered(thegame, i, p);
			if(i != -1 && p != -1)
			{
				thegame->sel1 = getHexagon(thegame, i, p);
				string sendString = "_buy base ";
				sendString += toString(thegame->sel1->i);
				sendString += ' ';
				sendString += toString(thegame->sel1->p);
				Client->Send(sendString.c_str());
				thegame->command = ' ';
			}
			thegame->sel1 = nullptr;
		}
		break;
	case 'M':
		if(thegame->sel1 == nullptr)
		{
			//may set i and p
			getHovered(thegame, i, p);
			if(i != -1 && p != -1)
			{
				thegame->sel1 = getHexagon(thegame, i, p);
				base* item = getItem(thegame, thegame->sel1->i, thegame->sel1->p);
				if(item == nullptr)
				{
					thegame->messages.AddMessage("You must select a tile with something on it!", 3);
					thegame->sel1 = nullptr;
				}
			}
			else
				thegame->messages.AddMessage("Please select a tile.", 3);
		}
		else if(thegame->sel2 == nullptr)
		{
			//may set i and p
			getHovered(thegame, i, p);
			if(i != -1 && p != -1)
			{
				thegame->sel2 = getHexagon(thegame, i, p);
				string sendString = "_move ";
				sendString += toString(thegame->sel1->i);
				sendString += ' ';
				sendString += toString(thegame->sel1->p);
				sendString += ' ';
				sendString += toString(thegame->sel2->i);
				sendString += ' ';
				sendString += toString(thegame->sel2->p);
				Client->Send(sendString.c_str());
				thegame->command = ' ';
				thegame->sel1 = nullptr;
				thegame->sel2 = nullptr;
			}
			else
				thegame->messages.AddMessage("Please select a tile.", 3);
		}
		break;
	case 'W':
		if(thegame->sel1 == nullptr)
		{
			//may set i and p
			getHovered(thegame, i, p);
			if(i != -1 && p != -1)
			{
				thegame->sel1 = getHexagon(thegame, i, p);
				string sendString = "_buy walker ";
				sendString += toString(thegame->sel1->i);
				sendString += ' ';
				sendString += toString(thegame->sel1->p);
				Client->Send(sendString.c_str());
				thegame->command = ' ';
			}
			thegame->sel1 = nullptr;
		}
		break;
	case 'T':
		if(thegame->sel1 == nullptr)
		{
			//may set i and p
			getHovered(thegame, i, p);
			if(i != -1 && p != -1)
			{
				thegame->sel1 = getHexagon(thegame, i, p);
				string sendString = "_buy turret ";
				sendString += toString(thegame->sel1->i);
				sendString += ' ';
				sendString += toString(thegame->sel1->p);
				Client->Send(sendString.c_str());
				thegame->command = ' ';
			}
			thegame->sel1 = nullptr;
		}
		break;
	default:
		//may set i and p
		getHovered(thegame, i, p);
		if(i != -1 && p != -1)
		{
			thegame->sel1 = getHexagon(thegame, i, p);
			string sendString = "_grab ";
			sendString += toString(thegame->sel1->i);
			sendString += ' ';
			sendString += toString(thegame->sel1->p);
			Client->Send(sendString.c_str());
			thegame->command = ' ';
		}
		thegame->sel1 = nullptr;
		break;
	}
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
		case (char)27:	//escape
			thegame->over = true;
			break;
		case (char)9:	//tab
			thegame->typing = true;
			break;
		case 't':
		case 'T':
			thegame->command = 'T';
			break;
		case 'w':
		case 'W':
			thegame->command = 'W';
			break;
		case 'b':
		case 'B':
			thegame->command = 'B';
			break;
		case 'm':
		case 'M':
			thegame->command = 'M';
			break;
		case 'g':
		case 'G':
			thegame->command = 'G';
			break;
		case ' ':
			getClient()->Send("_endturn");
			thegame->command = ' ';
			break;
		default:
			break;
		}
	}
}
