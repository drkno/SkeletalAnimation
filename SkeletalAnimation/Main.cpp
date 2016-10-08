#include <iostream>
#include <GL/freeglut.h>
#include "Stage.h"

using namespace std;

#include <assimp/cimport.h>
#include <assimp/types.h>
#include <assimp/scene.h>
#include "AssimpObject.h"

AssimpObject* person;

unsigned int tick;
aiVector3D scene_center;
Stage* stage = new Stage();
bool boneAnimation = false;

void initialise()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	if (boneAnimation) person = new AssimpObject("objects/x/wuson.x", true, true);
	else person = new AssimpObject("objects/bvh/01_01.bvh");
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, 1, 1.0, 1000.0);
}

void update(int value)
{
	person->updateAnimation(tick);
	tick = (tick + 1) % static_cast<int>(boneAnimation ? person->animDuration : 2130); // This is to stop the jumping animation jump through the blocks
	glutPostRedisplay();
	glutTimerFunc(person->secsPerTick * 1000, update, 0);
}

void display()
{
	float pos[4] = { 50, 50, 50, 1 };
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	auto rootPos = person->getRootPos();
	// scale the whole asset to fit into our view frustum 
	auto tmp = person->scene_max.x - person->scene_min.x;
	tmp = aisgl_max(person->scene_max.y - person->scene_min.y, tmp);
	tmp = aisgl_max(person->scene_max.z - person->scene_min.z, tmp);
	tmp = 1.f / tmp;
	gluLookAt(4, 1.5, boneAnimation ? 2 : -2, rootPos.x * tmp, rootPos.y * tmp, rootPos.z * tmp, 0, 1, 0);
	glLightfv(GL_LIGHT0, GL_POSITION, pos);

	glScalef(tmp, tmp, tmp);

	// center the model
	glTranslatef(-scene_center.x, -scene_center.y, -scene_center.z);


	if (boneAnimation)
	{
		glPushMatrix();
		glRotatef(90, 1, 0, 0);
	}
	person->render();
	if (boneAnimation)
		glPopMatrix();

	stage->floor();
	if (boneAnimation)
			stage->trees(tmp, tick);
	else
		stage->cubes();

	glutSwapBuffers();
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Assimp Test");
	glutInitContextVersion(4, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	initialise();
	glutDisplayFunc(display);
	glutTimerFunc(person->secsPerTick * 1000, update, 0);
	glutMainLoop();

	person->releaseResources();
	delete person;
	return 0;
}