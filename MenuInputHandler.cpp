#include "MenuInputHandler.h"
#include "game.h"
#include "NetworkClient.h"

bool getHovered(Game* thegame, int& i, int& p);
base* getHexagon(Game* thegame, int i, int p);

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
	thegame->buttonTimer = 10;
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
	case (char)27:
		thegame->over = true;
	}
}
