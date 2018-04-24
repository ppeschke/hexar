#pragma once
#include <string>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>

using namespace std;

class Message
{
public:
	Message(string msg, DWORD timer);
	~Message();

	void Run(DWORD deltaTime);
	bool isExpired();
	string getMessage();
	void setRect(int left, int top, int right, int bottom);
	RECT* getRect();

private:
	string msg;
	DWORD timer;
	bool expired;
	RECT rect;
};

