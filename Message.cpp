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

void Message::Run(float seconds)
{
	secondsTimer -= seconds;
	if(secondsTimer <= 0.0f)
		expired = true;
}

bool Message::isExpired()
{
	return expired;
}

string Message::getMessage()
{
	return msg;// +" " + to_string(secondsTimer);
}
