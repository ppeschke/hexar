#include "MessageQueue.h"

//from direct3d.cpp
void drawTextAt(const char* text, RECT* rect);
int figureTextLength(const char* str);

MessageQueue::MessageQueue()
{
	popped = false;
	added = false;
}


MessageQueue::~MessageQueue()
{
}

void MessageQueue::Render()
{
	RECT rect = {0, 0, 256, 300};
	drawTextAt(bigMessageString.c_str(), &rect);
	/*for(auto index = messages.begin(); index != messages.end(); ++index)
		drawTextAt(index->getMessage().c_str(), index->getRect());*/
}

void MessageQueue::Run(DWORD deltaTime)
{
	//update timers for messages
	for(auto index = messages.begin(); index != messages.end(); ++index)
		index->Run(deltaTime / 1000.0f);

	//remove expired messages
	while(!messages.empty() && messages.front().isExpired())
	{
		messages.pop_front();
		popped = true;
	}

	if(popped || added)
	{
		bigMessageString = "";
		for(auto index = messages.begin(); index != messages.end(); ++index)
		{
			bigMessageString += (*index).getMessage() + '\n';
		}
		popped = added = false;
	}
}

void MessageQueue::AddMessage(string msg, float seconds)
{
	messages.push_back(Message(msg, DWORD(seconds)));
	added = true;
}