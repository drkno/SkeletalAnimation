#pragma once

#include <math.h>
#include <GL/freeglut.h>
#include <string>
#include <GL/gl.h>

class BoxingRing
{
public:
	virtual ~BoxingRing()
	{
	}

	virtual void render() const;
};

