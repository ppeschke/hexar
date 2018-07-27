#include "InputEvent.h"



InputEvent::InputEvent(inputtype _type, char _pressed, float _x, float _z)
{
	type = _type;
	pressed = _pressed;
	x = _x;
	z = _z;
}


InputEvent::~InputEvent()
{
}
