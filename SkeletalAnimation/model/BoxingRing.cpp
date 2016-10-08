#include "BoxingRing.h"

void pillar(int x, int z)
{
	glPushMatrix();
	glTranslatef(x, 0, z);
	glRotatef(-90, 1, 0, 0);
	glutSolidCylinder(10, 125, 36, 5);
	glPopMatrix();
}

void rope(int y, float angle)
{
	glPushMatrix();
	glRotatef(angle, 0, 1, 0);
	glTranslatef(200, y, -200);
	glutSolidCylinder(3, 400, 3, 2);
	glPopMatrix();
}

void bag()
{
	glPushMatrix();
	glTranslatef(-100, 65, -100);
	glRotatef(-90, 1, 0, 0);
	glutSolidCylinder(20, 125, 36, 5);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-100, 190, -100);
	glRotatef(-90, 1, 0, 0);
	glutSolidCylinder(1, 125, 36, 5);
	glPopMatrix();
	glPopMatrix();
}

void BoxingRing::render() const
{
	pillar(200, 200);
	pillar(200, -200);
	pillar(-200, -200);
	pillar(-200, 200);
	rope(100, 0);
	rope(65, 0);
	rope(100, 90);
	rope(65, 90);
	rope(100, 180);
	rope(65, 180);
	rope(100, 270);
	rope(65, 270);
	bag();
}
