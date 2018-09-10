#pragma once
#include "InputHandler.h"
class GameInputHandler : public InputHandler
{
public:
	GameInputHandler(Game* g, NetworkClient* c);
	virtual ~GameInputHandler();

	virtual void handleMouseClick(InputEvent e);
	virtual void handleButtonPress(InputEvent e);
};

