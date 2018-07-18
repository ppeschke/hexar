#include "hexagon.h"

hexagon::hexagon(float _x, float _y, float _z, float _scale, color _c, MODEL* _m, int _i, int _p) : base(_x, _y, _z, _scale, _c, _m)
{
	i = _i;
	p = _p;
}

hexagon::~hexagon(void)
{
}


void hexagon::onStep(float seconds)
{
}

bool hexagon::coords(int _i, int _p)
{
	return (_i == i && _p == p);
}