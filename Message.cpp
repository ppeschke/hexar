#include "Message.h"

Message::Message(string m, DWORD t)
{
	msg = m;
	timer = t;
	expired = false;
}

Message::~Message()
{
}

void Message::Run(DWORD deltaTime)
{
	timer -= deltaTime;
	if(timer == 0 || timer > 2000000000)
		expired = true;
}

bool Message::isExpired()
{
	return expired;
}

string Message::getMessage()
{
	return msg;
}

void Message::setRect(int left, int top, int right, int bottom)
{
	SetRect(&this->rect, left, top, right, bottom);
}

RECT* Message::getRect()
{	//haha get wrecked!
	return &this->rect;
}