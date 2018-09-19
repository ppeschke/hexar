#pragma once
#include <string>
#include <list>
#include <fstream>
using namespace std;

#include "base.h"
#include "Camera.h"
#include "MessageQueue.h"
#include "MenuInputHandler.h"
#include "GameInputHandler.h"

class Game
{
public:
	Game(NetworkClient* c);
	~Game(void);

	void CleanObjs();

	list<base*> objects;
	list<MODEL*> models;
	bool over;
	bool setup;
	bool typing;
	int playerNum;
	color playercolors[6];
	color playerColor;
	base* arrow;
	base* sel1;
	base* sel2;
	char command;
	MessageQueue messages;
	int peschkes;
	Camera camera;
	ofstream logFile;
	string chatString;
	MenuInputHandler mih;
	GameInputHandler gih;
};
