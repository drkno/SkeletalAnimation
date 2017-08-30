#pragma once

#include <math.h>
#include <GL/freeglut.h>
#include <string>

class Floor
{
public:
	virtual ~Floor()
	{
	}

	Floor();
	virtual void render() const;
};
