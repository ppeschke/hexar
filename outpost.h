#pragma once
#include "rotator.h"
class outpost :	public rotator
{
public:
	outpost(float _x, float _y, float _z, float _scale, color _c, MODEL* _m, int _i, int _p);
	~outpost();

	virtual bool rcoords(int i, int p);
};

