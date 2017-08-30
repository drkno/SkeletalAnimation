#include "NormScene.h"

NormScene::NormScene()
{
	this->obj = new AssimpObject("objects/bvh/Boxing.bvh", true);
	this->boxingRing = new BoxingRing();
}

NormScene::~NormScene()
{
	delete this->boxingRing;
}

void NormScene::draw(float tmp, const aiVector3D& rootPos)
{
	gluLookAt(4, 1.0, -2, rootPos.x * tmp, rootPos.y * tmp, rootPos.z * tmp, 0, 1, 0);
	glScalef(tmp, tmp, tmp);
	glTranslatef(-scene_center.x, -scene_center.y, -scene_center.z);
	obj->render();
	boxingRing->render();
}

unsigned NormScene::getRefreshRate()
{
	return this->obj->getSecondsPerTick() * 1000;
}

void NormScene::updateTick()
{
	tick = (tick + 1) % static_cast<int>(this->obj->getAnimationDuration());
}

string NormScene::getTitle()
{
	return "BVH Skeletel Animation";
}
