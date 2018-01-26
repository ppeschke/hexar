#include "outpost.h"

const float PI = 3.14159f;

outpost::outpost(float _x, float _y, float _z, float _scale, color _c, MODEL* _m, int _i, int _p) : rotator(_x, _y, _z, _scale, _c, _m, _i, _p)
{
}


outpost::~outpost()
{
}

bool outpost::rcoords(int _i, int _p)
{
	return (_i == i && _p == p);
}