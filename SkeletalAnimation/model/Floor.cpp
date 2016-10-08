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
	glPushMatrix();
}

void Floor::cubes()
{
	for (auto i = 0; i < 3; i++)
	{
		glPushMatrix();
		glTranslatef(10, 2, (18 * i) + 1);
		glScalef(15, 2, 3);
		glutSolidCube(1.0);
		glPopMatrix();
	}
}

void Floor::trees(double scale, unsigned int tick)
{
	for (auto i = 0; i < 5; i++)
	{
		auto config = configs[i];
		auto shiftValue = tick * 0.2 - 85 * static_cast<int>(tick * 0.2 / 85);
		glPushMatrix();
		glTranslatef(config[0], 0, config[1] - shiftValue);
		glScalef(config[2], config[2], config[2]);

		glPushMatrix();
		glScalef(3.0 * scale, 6.0 * scale, 3.0 * scale);
		glPushMatrix();
		glRotatef(-90.0, 1, 0, 0);
		glutSolidCylinder(1.0, 1.0, 60, 60);
		glPopMatrix();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(0, 6 * scale, 0);
		glScalef(7.0 * scale, 30.0 * scale, 7.0 * scale);
		glPushMatrix();
		glRotatef(-90.0, 1, 0, 0);
		glutSolidCone(1.0, 1.0, 60, 60);
		glPopMatrix();
		glPopMatrix();

		glPopMatrix();
	}
}
