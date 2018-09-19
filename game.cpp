#include "game.h"
#include "model.h"

Game::Game(NetworkClient* c) : mih(this, c), gih(this, c)
{
	over = false;
	setup = false;
	typing = false;
	sel1 = NULL;
	sel2 = NULL;
	command = ' ';
	peschkes = 0;
	playerColor = white;
	chatString = "";
	arrow = nullptr;
}

Game::~Game(void)
{
	CleanObjs();
	for(list<MODEL*>::iterator index = models.begin(); index != models.end(); ++index)
	{
		delete (*index);
	}
}


void Game::CleanObjs()
{
	for(list<base*>::iterator index = objects.begin(); index != objects.end(); ++index)
	{
		if(*index)
			delete (*index);
	}
	objects.clear();
}
