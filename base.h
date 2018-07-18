#pragma once
class MODEL;

#include "color.h"

class base
{
public:
	base(float x, float y, float z, float scale, color c, MODEL* m);
	virtual ~base(void);

	virtual void onStep(float seconds) = 0;
	virtual bool coords(int, int);
	virtual bool rcoords(int, int);

	float x, y, z, scale, rotation;
	color c;
	MODEL* m;
	int i;
	int p;
};
