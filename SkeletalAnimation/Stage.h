#pragma once

#include <math.h>
#include <GL/freeglut.h>
#include <string>

class Stage
{
public:
	Stage();
	void floor();
	void cubes();
	void trees(double scale, unsigned int tick);
};

