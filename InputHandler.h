#pragma once

#include "InputEvent.h"

class Game;
class NetworkClient;

class InputHandler
{
public:
	InputHandler(Game* g, NetworkClient* client);
	virtual ~InputHandler();

	void handleMouseMove(InputEvent e);
	virtual void handleMouseClick(InputEvent e) = 0;
	virtual void handleButtonPress(InputEvent e) = 0;

protected:
	Game* thegame;
	NetworkClient* Client;
};

