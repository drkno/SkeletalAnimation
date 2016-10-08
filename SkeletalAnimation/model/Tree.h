#pragma once

#include <math.h>
#include <GL/freeglut.h>
#include <string>
#include <GL/gl.h>

class Tree
{
public:
	Tree(int, int);
	void render();
private:
	int angle, distance;
};

