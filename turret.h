#pragma once
#include "rotator.h"
class turret : public rotator
{
public:
	turret(float _x, float _y, float _z, float _scale, color _c, MODEL* _m, int _i, int _p);
	~turret();
};

