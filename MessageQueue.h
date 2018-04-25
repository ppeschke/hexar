#pragma once
#include <list>
using namespace std;

#include "Message.h"

class MessageQueue
{
public:
	MessageQueue();
	~MessageQueue();

	void Render();
	void Run(DWORD deltaTime);

	void AddMessage(string msg, float seconds);

private:
	list<Message> messages;
	string bigMessageString;
	bool popped;
	bool added;
};

