#include "MessageQueue.h"

//from direct3d.cpp
void drawTextAt(const char* text, RECT* rect);
int figureTextLength(const char* str);

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
	int height;
	//reset text rectangles for messages
	for(auto index = messages.begin(); index != messages.end(); ++index)
	{
		height = (index->getLines() * 20);
		//left, top, right, bottom (not length or height)
		index->setRect(0, top, 256, top + height);
		top += height;
	}
}

void MessageQueue::AddMessage(string msg, float seconds)
{
	//add code to add newline characters where appropriate
	int len = figureTextLength(msg.c_str());
	if(len < 256)
		messages.push_back(Message(msg, DWORD(seconds * 1000.0f), 1));
	else
		messages.push_back(Message(msg, DWORD(seconds * 1000.0f), 2));
}