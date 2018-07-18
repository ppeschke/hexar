#pragma once
#include "base.h"

class hexagon : public base
{
public:
	hexagon(float _x, float _y, float _z, float _scale, color _c, MODEL* _m, int ring, int count);
	~hexagon(void);

	virtual void onStep(float seconds);
	virtual bool coords(int, int);
};