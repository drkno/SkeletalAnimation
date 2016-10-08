#include "Stage.h"

Stage::Stage()
{
	float grey[4] = { 0.2, 0.2, 0.2, 1.0 };
	float white[4] = { 1.0, 1.0, 1.0, 1.0 };

	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 100);
	cameraAngle = 1;
	cameraViewAngle = 0;
}

void Stage::setAngle(int angle)
{
	cameraAngle = angle;
}

void Stage::draw()
{
	floor();
}

void Stage::changeCameraViewAngle(int change)
{
	cameraAngle = 0;
	cameraViewAngle -= change;
	if (cameraViewAngle < 0) cameraViewAngle = 359;
	if (cameraViewAngle >= 360) cameraViewAngle = 0;
}

void Stage::drawCamera()
{
	const float CDR = 3.14159265 / 180.0;		//Degrees to radians conversion factor

	switch (cameraAngle)
	{
	default:
	case 0: break;
	case 1: cameraViewAngle = 45; break;
	case 2: cameraViewAngle = 135; break;
	case 3: cameraViewAngle = 225; break;
	case 4: cameraViewAngle = 315; break;
	}

	float x = 392.0 * cosf((float)cameraViewAngle * CDR);
	float z = 196.0 * sinf((float)cameraViewAngle * CDR);
	gluLookAt(x, 50, z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

//-- Ground Plane --------------------------------------------------------
void Stage::floor()
{
	float white[4] = { 1., 1., 1., 1. };
	float black[4] = { 0 };
	float green[4] = { 0.0, 1.0, 0.0, 1.0 };

	glColor4f(0.048, 0.109, 0.048, 1.0);
	glNormal3f(0.0, 1.0, 0.0);
	glMaterialfv(GL_FRONT, GL_SPECULAR, black);  //No specular reflections from the floor.

	//The floor is made up of several tiny squares on a 200x200 grid. Each square has a unit size.
	glBegin(GL_QUADS);
	for (int i = -HALF_WIDTH; i < HALF_WIDTH; i++)
	{
		for (int j = -HALF_DEPTH; j < HALF_DEPTH; j++)
		{
			glVertex3f(i, 0.0, j);
			glVertex3f(i, 0.0, j + 1);
			glVertex3f(i + 1, 0.0, j + 1);
			glVertex3f(i + 1, 0.0, j);
		}
		if (i < 88 && i > 48) glColor4f(0.1, 0.1, 0.1, 1.0);
		else glColor4f(0.048, 0.109, 0.048, 1.0);
	}
	glEnd();

	glBegin(GL_POLYGON);
	glVertex3f(-4 * HALF_WIDTH, -0.01, -4 * HALF_DEPTH);
	glVertex3f(4 * HALF_WIDTH, -0.01, -4 * HALF_DEPTH);
	glVertex3f(4 * HALF_WIDTH, -0.01, 4 * HALF_DEPTH);
	glVertex3f(-4 * HALF_WIDTH, -0.01, 4 * HALF_DEPTH);
	glEnd();

	glMaterialfv(GL_FRONT, GL_SPECULAR, white);  //Enable specular reflections for remaining objects
}