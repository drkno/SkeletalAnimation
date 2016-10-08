#include "Scene.h"
#include <GL/freeglut.h>

Scene::Scene(): obj(nullptr), tick(0)
{
	floor = new Floor();
}

Scene::~Scene()
{
	delete obj;
	delete floor;
}

void Scene::updateTick()
{
	throw "This should have been overriden.";
}

void Scene::draw(float, const aiVector3D&)
{
	throw "This should have been overriden.";
}

void Scene::special(int, int, int) const
{
}

void Scene::key(unsigned char key, int, int) const
{
	if (key == 'Q' || key == 'q')
	{
		glutLeaveMainLoop();
	}
}

int Scene::getWidth() const
{
	return 600;
}

int Scene::getHeight() const
{
	return 600;
}

string Scene::getTitle() const
{
	return "Skeletal Animation";
}

unsigned Scene::getRefreshRate()
{
	throw "This should have been overriden.";
}

void Scene::initialise() const
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1.0, 1000.0);
}

void Scene::update()
{
	obj->update(tick);
	updateTick();
	glutPostRedisplay();
}

void Scene::display()
{
	float pos[4] = { 50, 50, 50, 1 };
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	auto scene_max = obj->getSceneMax(), scene_min = obj->getSceneMin();
	auto tmp = scene_max.x - scene_min.x;
	tmp = aisgl_max(scene_max.y - scene_min.y, tmp);
	tmp = aisgl_max(scene_max.z - scene_min.z, tmp);
	tmp = 1.f / tmp;

	glLightfv(GL_LIGHT0, GL_POSITION, pos);

	draw(tmp, obj->getObjectPosition());
	floor->render();
	glutSwapBuffers();
}