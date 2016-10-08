#include "BoneScene.h"

BoneScene::BoneScene()
{
	this->obj = new AssimpObject("objects/x/wuson.x", true);
	this->tree = new Tree(-15, 0);
	this->tree1 = new Tree(-30, 45);
	this->tree2 = new Tree(-10, 90);
	this->tree3 = new Tree(-30, 180);
	this->tree4 = new Tree(-15, 225);
	this->tree5 = new Tree(-10, 270);
	this->bisonOther = new AssimpObject("objects/x/wuson.x", false);
}

BoneScene::~BoneScene()
{
	delete this->tree;
	delete this->tree1;
	delete this->tree2;
	delete this->tree3;
	delete this->tree4;
	delete this->tree5;
	delete this->bisonOther;
}

void BoneScene::draw(float tmp, const aiVector3D& lookAtPos)
{
	gluLookAt(1.5, 0.5, 1.5, lookAtPos.x * tmp, lookAtPos.y * tmp, lookAtPos.z * tmp, 0, 1, 0);
	glScalef(tmp, tmp, tmp);
	glTranslatef(-scene_center.x, -scene_center.y, -scene_center.z);
	
	glPushMatrix();
	glRotatef(90, 1, 0, 0);		// rotate onto correct plane
	obj->render();
	glPopMatrix();

	this->tree->render();
	this->tree1->render();
	this->tree2->render();
	this->tree3->render();
	this->tree4->render();
	this->tree5->render();

	glPushMatrix();
	angle = (angle - 1) % 360;
	glRotatef(angle, 0, 1, 0);
	glTranslatef(-10, 0, -5);
	glScalef(1.25, 1.25, 1.25);
	glRotatef(80, 0, 1, 0);
	glRotatef(90, 1, 0, 0);		// rotate onto correct plane
	bisonOther->render();
	glPopMatrix();
}

unsigned BoneScene::getRefreshRate()
{
	return this->obj->getSecondsPerTick() * 1000;
}

void BoneScene::updateTick()
{
	tick = (tick + 1) % static_cast<int>(obj->getAnimationDuration());
}

string BoneScene::getTitle()
{
	return "Rigged Character Model Skeletel Animation";
}
