#include "action.h"
#include "game.h"

action parseMessage(Game* thegame, string b)
{
	action a;
	int index = b.find(" ");
	int index2;
	a.name = b.substr(0, index);
	if(a.name == "_grab")
	{
		index2 = b.find(" ", index + 1);
		a.int1 = atoi(b.substr(index + 1, index2 - index - 1).c_str());
		index = index2;
		index2 = b.find(" ", index + 1);
		a.int2 = atoi(b.substr(index + 1, index2 - index - 1).c_str());
		index = index2;
		a.int3 = atoi(b.substr(index + 1).c_str());

	}
	else if(a.name == "_move")
	{
		index2 = b.find(" ", index + 1);
		a.int1 = atoi(b.substr(index + 1, index2 - index - 1).c_str());
		index = index2;
		index2 = b.find(" ", index + 1);
		a.int2 = atoi(b.substr(index + 1, index2 - index - 1).c_str());
		index = index2;
		index2 = b.find(" ", index + 1);
		a.int3 = atoi(b.substr(index + 1, index2 - index - 1).c_str());
		index = index2;
		a.int4 = atoi(b.substr(index + 1).c_str());
	}
	else if(a.name == "_buy")
	{
		index2 = b.find(" ", index + 1);
		a.item = b.substr(index + 1, index2 - index - 1);
		index = index2;
		index2 = b.find(" ", index + 1);
		a.int1 = atoi(b.substr(index + 1, index2 - index - 1).c_str());
		index = index2;
		index2 = b.find(" ", index + 1);
		a.int2 = atoi(b.substr(index + 1, index2 - index - 1).c_str());
	}
	else if(a.name == "_endturn")
	{
	}
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
		a.int1 = atoi(b.substr(index + 1, index2 - index - 1).c_str());
		index = index2;
		index2 = b.find(" ", index + 1);
		a.int2 = atoi(b.substr(index + 1, index2 - index - 1).c_str());
	}
	else if(a.name == "_peschkes")
	{
		a.int1 = atoi(b.substr(index).c_str());
	}
	return a;
}