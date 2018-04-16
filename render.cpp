#include "game.h"
#include "model.h"

// Direct3D.cpp
void StartRender();
void EndRender();
void drawText(const char* text);
void drawPeschkes(const char* text);
void LoadModel(MODEL* Model, LPCTSTR File, int modelNum);
void SetCamera();
void AdjustCamera(float x, float y, float z);
void InitLight();
void DrawModel(MODEL* Model, float x, float y, float z, float scale, float rotation, color c);
string toString(int);

//render.cpp
void Render(Game* thegame);
void LoadGraphics(Game* thegame);

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

void Render(Game* thegame)
{
    StartRender();

	for(list<base*>::iterator index = thegame->objects.begin(); index != thegame->objects.end(); ++index)
	{
		DrawModel((*index)->m, (*index)->x, (*index)->y, (*index)->z, (*index)->scale, (*index)->rotation, (*index)->c);
	}
	if(thegame->setup)
		drawPeschkes((string("Peschkes: ") + toString(thegame->peschkes)).c_str());
	drawText(thegame->msg.c_str());
    EndRender();

    return;
}