#include "rotator.h"

const float PI = 3.14159f;

rotator::rotator(float _x, float _y, float _z, float _scale, color _c, MODEL* _m, int _i, int _p) : base(_x, _y, _z, _scale, _c, _m)
{
	i = _i;
	p = _p;
}

rotator::~rotator(void)
{
}

void rotator::onStep()
{
	rotation += 0.03f;
	if(rotation > 2 * PI)
		rotation = 0.0f;
}

bool rotator::rcoords(int _i, int _p)
{
	return (_i == i && _p == p);
}