#include "InputHandler.h"
#include "game.h"


InputHandler::InputHandler(Game* g, NetworkClient* client)
{
	this->thegame = g;
	Client = client;
}


InputHandler::~InputHandler()
{
}

void InputHandler::handleMouseMove(InputEvent e)
{
	if (thegame->arrow != nullptr)
	{
		thegame->arrow->x -= e.getX();
		thegame->arrow->z += e.getZ();
	}
}
