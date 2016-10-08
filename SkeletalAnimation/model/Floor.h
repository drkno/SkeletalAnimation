#pragma once

#include <math.h>
#include <GL/freeglut.h>
#include <string>

class Floor
{
public:
	Floor();
	void render() const;
	void cubes();
	void trees(double scale, unsigned int tick);
};
