#pragma once

#include <math.h>
#include <GL/freeglut.h>
#include <string>

class BoxingRing
{
public:
	virtual ~BoxingRing()
	{
	}

	virtual void render() const;
};

