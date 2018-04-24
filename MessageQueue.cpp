#include "MessageQueue.h"

//from direct3d.cpp
void drawTextAt(const char* text, RECT* rect);

MessageQueue::MessageQueue()
{
}


MessageQueue::~MessageQueue()
{
}

void MessageQueue::Render()
{
	for(auto index = messages.begin(); index != messages.end(); ++index)
		drawTextAt(index->getMessage().c_str(), index->getRect());
}

void MessageQueue::Run(DWORD deltaTime)
{
	//update timers for messages
	for(auto index = messages.begin(); index != messages.end(); ++index)
		index->Run(deltaTime);

	//remove expired messages
	while(!messages.empty() && messages.front().isExpired())
			messages.pop_front();

	int top = 0;
	int bottom;
	//reset text rectangles for messages
	for(auto index = messages.begin(); index != messages.end(); ++index)
	{
		bottom = top + 15;
		//left, top, right, bottom (not length or height)
		index->setRect(0, top, 256, bottom);
		top += 15;
	}
}

void MessageQueue::AddMessage(string msg, float seconds)
{
	//add code to add newline characters where appropriate
	messages.push_back(Message(msg, DWORD(seconds * 1000.0f)));
}