#include "MenuInputHandler.h"
#include "game.h"
#include "NetworkClient.h"

string toString(int);
bool getHovered(Game* thegame, int& i, int& p);
base* getHexagon(Game* thegame, int i, int p);
NetworkClient* getClient();

MenuInputHandler::MenuInputHandler(Game* g, NetworkClient* c) : InputHandler(g, c)
{
}

MenuInputHandler::~MenuInputHandler()
{
}

void MenuInputHandler::handleMouseClick(InputEvent e)
{
	int i, p;
	base* sel1 = nullptr;
	if (getHovered(thegame, i, p))
	{
		sel1 = getHexagon(thegame, i, p);
		string colors[] = { "white", "red", "green", "blue", "yellow", "orange", "cyan" };
		string msg = "_request " + colors[p + 1];
		Client->Send(msg.c_str());
	}
}

void MenuInputHandler::handleButtonPress(InputEvent e)
{
	switch (e.getPressed())
	{
	case (char)27:	//escape
		if(thegame->typing)
			thegame->typing = false;
		else
			thegame->over = true;
		break;
	case (char)9:	//tab
		thegame->typing = !thegame->typing;
		break;
	case (char)8:	//backspace
		if(thegame->typing && thegame->chatString.size() > 0)
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
