#include "Message.h"

Message::Message(string m, float t)
{
	msg = m;
	secondsTimer = t;
	expired = false;
}

Message::~Message()
{
}

void Message::Run(float deltaTime)
{
	secondsTimer -= deltaTime;
	if(secondsTimer <= 0)
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
