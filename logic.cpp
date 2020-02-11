#include <fstream>
using namespace std;

#include "action.h"
#include "game.h"
#include "hexagon.h"
#include "rotator.h"
#include "inputdata.h"
#include "NetworkClient.h"
#include "model.h"
#include "outpost.h"
#include "turret.h"

//global variables
//ofstream debugFile("debug.txt");

action parseMessage(Game* thegame, string buffer);
void doAction(Game* thegame, action);
string toString(int i);

MODEL* GetModel(Game* thegame, int modelNum)
{
	for(list<MODEL*>::iterator index = thegame->models.begin(); index != thegame->models.end(); ++index)
	{
		if((*index)->modelNum == modelNum)
			return (*index);
	}
	return NULL;
}

base* getHexagon(Game* thegame, int i, int p)
{
	for(list<base*>::iterator index = thegame->objects.begin(); index != thegame->objects.end(); ++index)
	{
		if((*index)->coords(i, p) && typeid(hexagon) == typeid(**index))
			return (*index);
	}
	return nullptr;
}

base* getItem(Game* thegame, int i, int p)
{
	for(list<base*>::reverse_iterator rindex = thegame->objects.rbegin(); rindex != thegame->objects.rend(); ++rindex)
	{
		if((*rindex)->rcoords(i, p))
			return (*rindex);
	}
	return nullptr;
}

float distance(float x1, float y1, float x2, float y2)
{
	return sqrt(pow(y2 - y1, 2) + pow(x2 - x1, 2));
}

bool nextTo(base* a, base* b)
{
	return distance(a->x, a->z, b->x, b->z) < 3.0f;
}

bool getHovered(Game* thegame, int& i, int& p)
{
	list<base*> close;
	bool found = false;
	float dist = 1000.0f;
	for(list<base*>::iterator index = thegame->objects.begin(); index != thegame->objects.end(); ++index)
	{
		if(nextTo(thegame->arrow, (*index)) && (*index) != thegame->arrow)
		{
			close.insert(close.end(), (*index));
			found = true;
		}
	}
	for(list<base*>::iterator index = close.begin(); found && index != close.end(); ++index)
	{
		float temp = distance(thegame->arrow->x, thegame->arrow->z, (*index)->x, (*index)->z); 
		if(temp < dist)
		{
			dist = temp;
			i = (*index)->i;
			p = (*index)->p;
		}
	}
	return found;
}

void SetupMenu(Game* thegame)
{
	thegame->camera.SetCamera(40.0f, thegame->camera.PI / 2, 3 * thegame->camera.PI / 8);
	base* temp = NULL;
	float x = 7.5f + 1.25f;
	for(int i = 0; i < 6; ++i)
	{
		temp = new hexagon(x, 0.0f, 0.0f, 1.0f, (color)(i + 1), GetModel(thegame, 1), 0, i);
		thegame->objects.insert(thegame->objects.end(), temp);
		x -= 2.5f;
	}
	temp = new rotator(0.0f, 2.0f, 0.0f, 1.0f, white, GetModel(thegame, 5), -1, -1);
	thegame->objects.insert(thegame->objects.end(), temp);
	thegame->arrow = temp;
	return;
}

void InitGame(Game* thegame, int playerNum, int rings)
{
	int count = rings;
	float x, rx, rz;
	rx = 0.0f + 1.5f * (rings - 1);
	rz = 0.0f - 2.0f * (rings - 1);
	base* temp;
	for(int i = 0; i < rings * 2 - 1; ++i)
	{
		x = rx;
		for(int p = 0; p < count; ++p)
		{
			temp = new hexagon(x, 0.0f, rz, 1.0f, white, GetModel(thegame, 1), i, p);
			thegame->objects.insert(thegame->objects.end(), temp);
			x -= 2.5;
		}
		count += (i + 1 < rings ? 1 : -1);
		rx = rx + (i + 1 < rings? 1.5f : -1.5f);
		rz += 2.0;
	}
	thegame->playerNum = playerNum;
	thegame->arrow = new rotator(0.0f, 2.0f, 0.0f, 1.0f, thegame->playerColor, GetModel(thegame, 5), 0, 0);
	thegame->objects.insert(thegame->objects.end(), thegame->arrow);
	return;
}

float randomNumber(float low, float high)
{
	return low + ((float)rand() / (float)RAND_MAX) * (high - low);
}

void RunFrame(Game* thegame, NetworkClient* Client, float seconds)
{
	int i = -1, p = -1;
	static base* hover = nullptr;
	bool commandComplete = false;

	Client->locker.lock();
	if(Client->messages.size() > 0)
	{
		list<string>::iterator msg = Client->messages.begin();
		if((*msg)[0] == '_')
		{
			action a = parseMessage(thegame, *msg);
			doAction(thegame, a);
		}
		else
			thegame->messages.AddMessage(*msg, 5);
		Client->messages.pop_front();
	}
	Client->locker.unlock();
	thegame->messages.Run(seconds);

	thegame->camera.Run();

	for(list<base*>::iterator index = thegame->objects.begin(); index != thegame->objects.end(); ++index)
	{
		(*index)->onStep(seconds);
	}
    return;
}

void RunMenuFrame(Game* thegame, NetworkClient* Client, float seconds)
{
	action tempAction;
	Client->locker.lock();
	if(Client->messages.size() > 0)
	{
		tempAction = parseMessage(thegame, *(Client->messages.begin()));
		Client->messages.pop_front();
		//debugFile << "tempAction: " << tempAction.item << ", " << tempAction.name << endl;
		doAction(thegame, tempAction);
	}
	Client->locker.unlock();
	thegame->messages.Run(seconds);
	thegame->arrow->onStep(seconds);

	thegame->camera.Run();

	return;
}

string toString(int i)
{
	char temp[10];
	_itoa_s(i, temp, 10);
	return (string)temp;
}