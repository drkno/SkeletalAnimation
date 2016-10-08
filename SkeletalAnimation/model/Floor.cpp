#include "Floor.h"
#include <GL/gl.h>

auto configs = new float[5][3]{
	{-21, 20, 0.9f},
	{3,41, 0.7f},
	{-14,35, 1.1f},
	{-19, 20, 1.0f},
	{-3, 61, 0.75f}
};

Floor::Floor()
{
}

void Floor::render() const
{
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glColor3f(0.7, 0.7, 0.7);

	glBegin(GL_QUADS);

	for (int x = -100; x < 100; x++)
	{
		for (int y = -100; y < 100; y++)
		{
			glNormal3f(0, 1, 0);
			glVertex3f(x, 0, y);

			glNormal3f(0, 1, 0);
			glVertex3f(x, 0, y + 1);

			glNormal3f(0, 1, 0);
			glVertex3f(x + 1, 0, y + 1);

			glNormal3f(0, 1, 0);
			glVertex3f(x + 1, 0, y);
		}
	}
	glEnd();
	glEnable(GL_LIGHTING);
	glPopMatrix();
}

