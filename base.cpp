#include "base.h"
#include "model.h"

base::base(float _x, float _y, float _z, float _s, color _c, MODEL* _m)
{
	x = _x;
	y = _y;
	z = _z;
	scale = _s;
	c = _c;
	m = _m;
	rotation = 0.0f;
}

base::~base(void)
{
}

bool base::coords(int, int)
{
	return false;
}

bool base::rcoords(int, int)
{
	return false;
}