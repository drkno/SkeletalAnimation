#include "Floor.h"
#include <algorithm>
using namespace std;

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
	
	glNormal3f(0, 1, 0);
	glVertex3f(-500, 0, -500);

	glNormal3f(0, 1, 0);
	glVertex3f(-500, 0, 500);

	glNormal3f(0, 1, 0);
	glVertex3f(500, 0, 500);

	glNormal3f(0, 1, 0);
	glVertex3f(500, 0, -500);

	glEnd();
	glEnable(GL_LIGHTING);
	glPopMatrix();
}