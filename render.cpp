#include "global.h"

void LoadGraphics(Game* thegame)
{
	MODEL* temp = new MODEL;
    LoadModel(temp, TEXT("hex6.x"), 1);
	thegame->models.insert(thegame->models.end(), temp);
	temp = new MODEL;
	LoadModel(temp, TEXT("base.x"), 2);
	thegame->models.insert(thegame->models.end(), temp);
	temp = new MODEL;
	LoadModel(temp, TEXT("turret.x"), 3);
	thegame->models.insert(thegame->models.end(), temp);
	temp = new MODEL;
	LoadModel(temp, TEXT("walker.x"), 4);
	thegame->models.insert(thegame->models.end(), temp);
	temp = new MODEL;
	LoadModel(temp, TEXT("arrow.x"), 5);
	thegame->models.insert(thegame->models.end(), temp);
    SetCamera();
	AdjustCamera(1.5f, 30.0f, 10.0f);
	InitLight();

    return;
}

void Render(Game* thegame, client* Client)
{
    StartRender();

	for(list<base*>::iterator index = thegame->objects.begin(); index != thegame->objects.end(); ++index)
	{
		DrawModel((*index)->m, (*index)->x, (*index)->y, (*index)->z, (*index)->scale, (*index)->rotation, (*index)->c);
	}

	drawText(thegame->msg.c_str());
    EndRender();

    return;
}