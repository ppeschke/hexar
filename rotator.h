#pragma once
#include "base.h"

class rotator : public base
{
public:
	rotator(float _x, float _y, float _z, float _scale, color _c, MODEL* _m, int _i, int _p);
	~rotator(void);

	virtual void onStep(float seconds);
	virtual bool rcoords(int, int);
};
