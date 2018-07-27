#ifndef INPUTPROCESSOR_H
#define INPUTPROCESSOR_H
#include "InputEvent.h"
#include "game.h"

class InputProcessor
{
public:
	InputProcessor(Game* tg);
	~InputProcessor();

	void ProcessInputs();
	InputEvent TranslateKeyboardEvent(short code, bool shift);
	list<InputEvent>* getEvents();

private:
	BYTE* Keys;
	BYTE* prevKeys;
	float x, y, z;
	float mouseDownX, mouseDownY;
	Game* thegame;
	bool Clicked;
	bool caps;
	list<InputEvent> events;
};

#endif