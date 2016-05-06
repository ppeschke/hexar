#pragma once
#include <string>
#include <list>
using namespace std;

#include "base.h"

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
	base* arrow;
	int buttonTimer;
	base* sel1;
	base* sel2;
	char command;
	string msg;
};
