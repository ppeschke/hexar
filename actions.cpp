#include "action.h"
#include "game.h"
#include "outpost.h"

base* getHexagon(Game* thegame, int i, int p);
MODEL* GetModel(Game* thegame, int modelNum);
string toString(int i);
base* getItem(Game* thegame, int i, int p);
void InitGame(Game* thegame, int playerNum, int rings);

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
	else if(tempAction.name == "_granted")
	{
		string colors[7] = {"white", "red", "green", "blue", "yellow", "orange", "cyan"};
		int p;
		for(p = 0; p < 7; ++p)
		{
			if(colors[p] == tempAction.item)
				break;
		}
		thegame->playerColor = (color)p;
	}
	else if(tempAction.name == "_grab")
	{
		base* temp = getHexagon(thegame, tempAction.int1, tempAction.int2);
		if(temp)
			temp->c = (color)tempAction.int3;
	}
	else if(tempAction.name == "_buy")
	{
		if(tempAction.item == "base")
		{
			base* hex = getHexagon(thegame, tempAction.int1, tempAction.int2);
			base* temp = new outpost(hex->x, hex->y, hex->z, 0.3f, hex->c, GetModel(thegame, 2), tempAction.int1, tempAction.int2);
			thegame->objects.insert(thegame->objects.end(), temp);
		}
		else if(tempAction.item == "walker")
		{
			base* hex = getHexagon(thegame, tempAction.int1, tempAction.int2);
			base* temp = new rotator(hex->x, hex->y - 1.0f, hex->z, 0.3f, hex->c, GetModel(thegame, 4), tempAction.int1, tempAction.int2);
			thegame->objects.insert(thegame->objects.end(), temp);
		}
		else if(tempAction.item == "turret")
		{
			base* hex = getHexagon(thegame, tempAction.int1, tempAction.int2);
			base* temp = new rotator(hex->x, hex->y - 1.0f, hex->z, 0.7f, hex->c, GetModel(thegame, 3), tempAction.int1, tempAction.int2);
			thegame->objects.insert(thegame->objects.end(), temp);
		}
	}
	else if(tempAction.name == "_setup")
	{
		thegame->CleanObjs();
		InitGame(thegame, tempAction.int1, tempAction.int2);
	}
	else if(tempAction.name == "_play")
		thegame->setup = true;
	else if(tempAction.name == "_peschkes")
	{
		thegame->peschkes = tempAction.int1;
	}
	else if(tempAction.name == "_move")
	{
		base* item = getItem(thegame, tempAction.int1, tempAction.int2);
		base* hex = getHexagon(thegame, tempAction.int3, tempAction.int4);
		if(!(item && hex))
			exit(EXIT_FAILURE);
		item->i = tempAction.int3;
		item->p = tempAction.int4;
		item->x = hex->x;
		item->z = hex->z;
		hex->c = item->c;
	}
}