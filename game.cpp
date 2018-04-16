#include "game.h"
#include "model.h"

Game::Game(void)
{
	over = false;
	setup = false;
	buttonTimer = 0;
	sel1 = NULL;
	sel2 = NULL;
	command = ' ';
	msg = "no messages";
	peschkes = 0;
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