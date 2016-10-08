#include "SceneObject.h"

GLUquadric* SceneObject::q;

void SceneObject::display()
{
	glPushMatrix();
	draw();
	glPopMatrix();
}

void SceneObject::draw()
{
	throw "This class is abstract and should be overridden.";
}

void SceneObject::setQuad(GLUquadric* quad)
{
	q = quad;
}

GLUquadric* SceneObject::getQuad()
{
	return q;
}

void SceneObject::drawCamera()
{
	throw "Cannot attach camera to this object.";
}

void SceneObject::setAngle(int)
{
	throw "Cannot set angle on this object.";
}

void SceneObject::changeCameraViewAngle(int)
{
	throw "Cannot set camera view on this object.";
}