#pragma once
#include <string>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>

using namespace std;

class Message
{
public:
	Message(string msg, float seconds);
	~Message();

	void Run(float deltaTime);
	bool isExpired();
	string getMessage();

private:
	string msg;
	float secondsTimer;
	bool expired;
};

