#ifndef INPUTPROCESSOR_H
#define INPUTPROCESSOR_H

#include "game.h"

class InputProcessor
{
public:
	InputProcessor(Game* tg);
	~InputProcessor();

	void ProcessInputs();
	void ProcessEvents();

private:
	BYTE* Keys;
	BYTE* prevKeys;
	float x, y, z;
	float mouseDownX, mouseDownY;
	Game* thegame;
	bool Clicked;
	list<short> events;
};

#endif