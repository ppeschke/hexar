#pragma once

enum inputtype {keyboard, mouse};

class InputEvent
{
public:
	InputEvent(inputtype type, char pressed, float _x, float _z);
	~InputEvent();
	inputtype getType() { return type; }
	char getPressed() { return pressed; }
	float getX() { return x; }
	float getZ() { return z; }

private:
	inputtype type;
	char pressed;
	float x;
	float z;
};

