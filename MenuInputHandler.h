#pragma once
#include "InputHandler.h"
class MenuInputHandler : public InputHandler
{
public:
	MenuInputHandler(Game* g, NetworkClient* c);
	virtual ~MenuInputHandler();

	virtual void handleMouseClick(InputEvent e);
	virtual void handleButtonPress(InputEvent e);
};

