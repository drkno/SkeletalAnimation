#include "Tree.h"

Tree::Tree(int distance, int angle)
{
	this->distance = distance;
	this->angle = angle;
}

void Tree::render()
{
	this->angle = (this->angle - 1) % 360;
	glPushMatrix();
	glRotatef(angle, 0, 1, 0);
	glTranslatef(this->distance, 0, 0);
	glScalef(0.5, 0.5, 0.5);
	glRotatef(-90, 1, 0, 0);
	glutSolidCylinder(0.6, 2, 8, 2);
	glTranslatef(0, 0, 2);
	glutSolidCone(3, 5, 6, 2);
	glTranslatef(0, 0, 2);
	glutSolidCone(2.5, 4, 4, 2);
	glPopMatrix();
}
