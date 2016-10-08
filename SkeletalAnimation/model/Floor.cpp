#include "Floor.h"
#include <GL/gl.h>

Floor::Floor()
{
}

// taken from my FavershamExpress repo http://github.com/mrkno/FavershamExpress
void Floor::render() const
{
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glColor3f(0.5, 0.5, 0.5);
	glBegin(GL_QUADS);
	for (auto x = -100; x < 100; x++)
	{
		for (auto y = -100; y < 100; y++)
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
