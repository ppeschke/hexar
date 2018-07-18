#pragma once
#include <string>
#include <list>
#include <fstream>
using namespace std;

#include "base.h"
#include "Camera.h"
#include "MessageQueue.h"

class Game
{
public:
	Game(void);
	~Game(void);

	void CleanObjs();

	list<base*> objects;
	list<MODEL*> models;
	bool over;
	bool setup;
	int playerNum;
	color playercolors[6];
	color playerColor;
	base* arrow;
	int buttonTimer;
	base* sel1;
	base* sel2;
	char command;
	MessageQueue messages;
	int peschkes;
	Camera camera;
	ofstream logFile;
};
