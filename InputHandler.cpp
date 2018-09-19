#include "InputHandler.h"
#include "game.h"
bool getHovered(Game* thegame, int& i, int& p);
base* getHexagon(Game* thegame, int i, int p);
base* getItem(Game* thegame, int i, int p);

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
	static base* hover = nullptr;
	int i = -1, p = -1;
	if (thegame->arrow != nullptr)
	{
		thegame->arrow->x -= e.getX();
		thegame->arrow->z += e.getZ();

		//mouse hover code
		base* prevHover = hover;
		//may set i and p
		getHovered(thegame, i, p);
		if(i != -1 && p != -1)
		{
			if(hover = getHexagon(thegame, i, p))
			{
				hover->y = 1.0f;
				if(base* item = getItem(thegame, i, p))
					item->y = hover->y + 1.0f;
			}
		}
		if(prevHover != nullptr && prevHover != hover)
		{
			prevHover->y = 0.0f;
			if(base* item = getItem(thegame, prevHover->i, prevHover->p))
				item->y = prevHover->y + 1.0f;
		}
	}
}
