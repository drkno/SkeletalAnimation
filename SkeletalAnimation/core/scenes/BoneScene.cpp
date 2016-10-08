#include "BoneScene.h"
#include "../../model/AssimpObject.h"

BoneScene::BoneScene()
{
	this->obj = new AssimpObject("objects/x/wuson.x", true);
}

BoneScene::~BoneScene()
{
}

void BoneScene::draw(float tmp, const aiVector3D& rootPos)
{
	gluLookAt(4, 1.5, 2, rootPos.x * tmp, rootPos.y * tmp, rootPos.z * tmp, 0, 1, 0);
	glScalef(tmp, tmp, tmp);
	glTranslatef(-scene_center.x, -scene_center.y, -scene_center.z);
	
	glPushMatrix();
	glRotatef(90, 1, 0, 0);		// rotate onto correct plane
	obj->render();
	glPopMatrix();

	//stage->floor();
	//stage->trees(tmp, tick);
}

unsigned BoneScene::getRefreshRate()
{
	return this->obj->getSecondsPerTick() * 1000;
}

void BoneScene::updateTick()
{
	tick = (tick + 1) % static_cast<int>(obj->getAnimationDuration());
}
