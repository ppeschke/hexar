#include <cmath>
#include <fstream>
using namespace std;

#include "action.h"
#include "game.h"
#include "hexagon.h"
#include "rotator.h"
#include "inputdata.h"
#include "NetworkClient.h"

//direct3d.cpp prototypes
void AdjustCamera(float x, float y, float z);

// global variables
float camXAngle;
float camYAngle;
float camZoom;
const float PI = 3.14159f;
ofstream debugFile("debug.txt");

action parseMessage(Game* thegame, string buffer);
void doAction(Game* thegame, action);

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
		if((*index)->coords(i, p))
			return (*index);
	}
	return NULL;
}

float distance(float x1, float y1, float x2, float y2)
{
	return sqrt(pow(y2 - y1, 2) + pow(x2 - x1, 2));
}

bool nextTo(base* a, base* b)
{
	return distance(a->x, a->z, b->x, b->z) < 3.0f;
}

bool getSelected(Game* thegame, int& i, int& p)
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
	for(list<base*>::iterator index = close.begin(); index != close.end(); ++index)
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

void MainMenu(Game* thegame)
{
	camZoom = 40.0f;
	camXAngle = PI / 2;
	camYAngle = 3 * PI / 8;
	AdjustCamera((float)cos((double)camXAngle) * camZoom * (float)cos((double)camYAngle),
				 (float)sin((double)camYAngle) * camZoom,
				 (float)sin((double)camXAngle) * camZoom * (float)cos((double)camYAngle));
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
			//Render(thegame);
			x -= 2.5;
		}
		count += (i + 1 < rings ? 1 : -1);
		rx = rx + (i + 1 < rings? 1.5f : -1.5f);
		rz += 2.0;
	}
	thegame->playerNum = playerNum;
	thegame->arrow = new rotator(0.0f, 2.0f, 0.0f, 1.0f, white, GetModel(thegame, 5), 0, 0);
	thegame->objects.insert(thegame->objects.end(), thegame->arrow);
	return;
}

float randomNumber(float low, float high)
{
	return low + ((float)rand() / (float)RAND_MAX) * (high - low);
}

void Logic(Game* thegame, INPUTDATA* InputData, NetworkClient* Client)
{
	int i, p;
	static base* hover = NULL;
    // for every millisecond...
	thegame->arrow->onStep();

	if(InputData->Esc)
		thegame->over = true;
	if(InputData->MoveUp)
		camYAngle += 0.03f;
	if(InputData->MoveDown)
		camYAngle -= 0.03f;
	if(InputData->MoveLeft)
		camXAngle -= 0.03f;
	if(InputData->MoveRight)
		camXAngle += 0.03f;
	if(thegame->buttonTimer == 0 && InputData->B)
	{
		if(thegame->sel1)
			thegame->sel1->y = 0.0f;
		if(thegame->sel2)
			thegame->sel2->y = 0.0f;
		thegame->sel1 = NULL;
		thegame->sel2 = NULL;
		thegame->command = 'B';
		thegame->buttonTimer = 10;
	}
	if(thegame->buttonTimer == 0 && InputData->T)
	{
		if(thegame->sel1)
			thegame->sel1->y = 0.0f;
		if(thegame->sel2)
			thegame->sel2->y = 0.0f;
		thegame->sel1 = NULL;
		thegame->sel2 = NULL;
		thegame->command = 'T';
		thegame->buttonTimer = 10;
	}
	if(thegame->buttonTimer == 0 && InputData->W)
	{
		if(thegame->sel1)
			thegame->sel1->y = 0.0f;
		if(thegame->sel2)
			thegame->sel2->y = 0.0f;
		thegame->sel1 = NULL;
		thegame->sel2 = NULL;
		thegame->command = 'W';
		thegame->buttonTimer = 10;
	}
	if(thegame->buttonTimer == 0 && InputData->M)
	{
		if(thegame->sel1)
			thegame->sel1->y = 0.0f;
		if(thegame->sel2)
			thegame->sel2->y = 0.0f;
		thegame->sel1 = NULL;
		thegame->sel2 = NULL;
		thegame->command = 'M';
		thegame->buttonTimer = 10;
	}
	if(thegame->buttonTimer == 0 && InputData->G)
	{
		if(thegame->sel1)
			thegame->sel1->y = 0.0f;
		if(thegame->sel2)
			thegame->sel2->y = 0.0f;
		thegame->sel1 = NULL;
		thegame->sel2 = NULL;
		thegame->command = 'G';
		thegame->buttonTimer = 10;
	}
	if(thegame->buttonTimer == 0 && InputData->EndTurn)
	{
		if(thegame->sel1)
			thegame->sel1->y = 0.0f;
		if(thegame->sel2)
			thegame->sel2->y = 0.0f;
		thegame->sel1 = NULL;
		thegame->sel2 = NULL;
		thegame->command = ' ';
		thegame->buttonTimer = 10;
	}

	if(camXAngle > 2 * PI)
		camXAngle = 0.0f;
	if(camXAngle < 0.0f)
		camXAngle = 2 * PI;
	if(camYAngle > 3 * PI / 8)
		camYAngle = 3 * PI / 8;
	if(camYAngle < PI / 6)
		camYAngle = PI / 6;

	camZoom -= InputData->MouseZ / 50.0f;

	if(camZoom < 20.0f)
		camZoom = 20.0f;
	if(camZoom > 90.0f)
		camZoom = 90.0f;

	thegame->arrow->x -= InputData->MouseX / 20.0f;
	thegame->arrow->z += InputData->MouseY / 20.0f;

	base* hovtemp = hover;
	if(getSelected(thegame, i, p))
	{
		if(hover = getHexagon(thegame, i, p))
		{
			if(hovtemp)
				hovtemp->y = 0.0f;
			hover->y = 1.0f;
		}
	}
	else
	{
		if(hover)
			hover->y = 0.0f;
		hover = NULL;
	}

	if(InputData->MouseButton && !thegame->buttonTimer && thegame->command != ' ')
	{
		if(thegame->buttonTimer == 0)
		{
			thegame->buttonTimer = 10;
			if(getSelected(thegame, i, p))
			{
				if(thegame->command == 'M' && thegame->sel1)
				{
					thegame->sel2 = getHexagon(thegame, i, p);
				}
				else
					thegame->sel1 = getHexagon(thegame, i, p);
			}
		}
	}
	if(thegame->sel1)
		thegame->sel1->y = 2.0f;

	AdjustCamera((float)cos((double)camXAngle) * camZoom * (float)cos((double)camYAngle),
				 (float)sin((double)camYAngle) * camZoom,
				 (float)sin((double)camXAngle) * camZoom * (float)cos((double)camYAngle));
	if(thegame->buttonTimer > 0)
		thegame->buttonTimer -= 1;

    return;
}

void MenuLogic(Game* thegame, INPUTDATA* InputData, NetworkClient* Client)
{
	int i, p;
	static base* hover = NULL;
	base* sel1 = NULL;
    // for every millisecond...

	action tempAction;
	if(Client->hasMessage())
	{
		tempAction = parseMessage(thegame, Client->getAMessage());
		debugFile << "tempAction: " << tempAction.item << ", " << tempAction.name << endl;
		doAction(thegame, tempAction);
	}
	thegame->arrow->onStep();

	if(InputData->Esc)
		thegame->over = true;
	if(InputData->MoveUp)
		camYAngle += 0.03f;
	if(InputData->MoveDown)
		camYAngle -= 0.03f;
	if(InputData->MoveLeft)
		camXAngle -= 0.03f;
	if(InputData->MoveRight)
		camXAngle += 0.03f;

	if(camXAngle > 2 * PI)
		camXAngle = 0.0f;
	if(camXAngle < 0.0f)
		camXAngle = 2 * PI;
	if(camYAngle > 3 * PI / 8)
		camYAngle = 3 * PI / 8;
	if(camYAngle < PI / 6)
		camYAngle = PI / 6;

	camZoom -= InputData->MouseZ / 50.0f;

	if(camZoom < 20.0f)
		camZoom = 20.0f;
	if(camZoom > 90.0f)
		camZoom = 90.0f;

	thegame->arrow->x -= InputData->MouseX / 20.0f;
	thegame->arrow->z += InputData->MouseY / 20.0f;

	base* hovtemp = hover;
	if(getSelected(thegame, i, p))
	{
		if(hover = getHexagon(thegame, i, p))
		{
			if(hovtemp)
				hovtemp->y = 0.0f;
			hover->y = 1.0f;
		}
	}
	else
	{
		if(hover)
			hover->y = 0.0f;
		hover = NULL;
	}

	if(InputData->MouseButton && !thegame->buttonTimer)
	{
		if(thegame->buttonTimer == 0)
		{
			thegame->buttonTimer = 10;
			if(getSelected(thegame, i, p))
			{
				sel1 = getHexagon(thegame, i, p);
				string colors[] = {"white", "red", "green", "blue", "yellow", "orange", "cyan"};
				string msg = "_request " + colors[p + 1];
				Client->Send(msg.c_str());
			}
		}
	}
	if(sel1)
		sel1->y = 2.0f;

	AdjustCamera((float)cos((double)camXAngle) * camZoom * (float)cos((double)camYAngle),
				 (float)sin((double)camYAngle) * camZoom,
				 (float)sin((double)camXAngle) * camZoom * (float)cos((double)camYAngle));
	if(thegame->buttonTimer > 0)
		thegame->buttonTimer -= 1;
	return;
}

action parseMessage(Game* thegame, string b)
{
	thegame->msg = b;
	action a;
	int index = b.find(" ");
	int index2;
	a.name = b.substr(0, index);
	if(a.name == "_grab")
	{
		index2 = b.find(" ", index + 1);
		a.int1 = atoi(b.substr(index, index2 - index - 1).c_str());
		index = index2;
		index2 = b.find(" ", index + 1);
		a.int2 = atoi(b.substr(index, index2 - index - 1).c_str());
		index = index2;
		a.int3 = atoi(b.substr(index).c_str());
		
	}
	else if(a.name == "_move")
	{
		index2 = b.find(" ", index + 1);
		a.int1 = atoi(b.substr(index, index2 - index - 1).c_str());
		index = index2;
		index2 = b.find(" ", index + 1);
		a.int2 = atoi(b.substr(index, index2 - index - 1).c_str());
		index = index2;
		index2 = b.find(" ", index + 1);
		a.int3 = atoi(b.substr(index, index2 - index - 1).c_str());
		index = index2;
		a.int4 = atoi(b.substr(index).c_str());
	}
	else if(a.name == "_buy")
	{
		index2 = b.find(" ", index + 1);
		a.item = b.substr(index + 1, index2 - index - 1);
		index = index2;
		index2 = b.find(" ", index + 1);
		a.int1 = atoi(b.substr(index, index2 - index - 1).c_str());
		index = index2;
		index2 = b.find(" ", index + 1);
		a.int2 = atoi(b.substr(index, index2 - index - 1).c_str());
	}
	else if(a.name == "_endturn")
	{}
	else if(a.name == "_granted")
	{
		a.item = b.substr(index + 1);
	}
	else if(a.name == "_taken")
	{
		a.item = b.substr(index + 1);
	}
	else if(a.name == "_setup")
	{
		index2 = b.find(" ", index + 1);
		a.int1 = atoi(b.substr(index, index2 - index - 1).c_str());
		index = index2;
		index2 = b.find(" ", index + 1);
		a.int2 = atoi(b.substr(index, index2 - index - 1).c_str());
	}
	return a;
}

void doAction(Game* thegame, action tempAction)
{
	if(tempAction.name == "_taken")
	{
		string colors[7] = {"white", "red", "green", "blue", "yellow", "orange", "cyan"};
		int p;
		for(p = 0; p < 7; ++p)
		{
			if(colors[p] == tempAction.item)
				break;
		}
		base* ptr = getHexagon(thegame, 0, p - 1);
		delete ptr;
		thegame->objects.remove(ptr);
	}
	else if(tempAction.name == "_grab")
	{
		base* temp = getHexagon(thegame, tempAction.int1, tempAction.int2);
		if(temp)
			temp->c = (color)tempAction.int3;
	}
	else if(tempAction.name == "_setup")
	{
		thegame->CleanObjs();
		InitGame(thegame, tempAction.int1, tempAction.int2);
		thegame->setup = true;
	}
}