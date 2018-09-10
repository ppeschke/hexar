#include "GameInputHandler.h"
#include "game.h"

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
	switch (e.getPressed())
	{
	case (char)27:
		thegame->over = true;
	}
}
