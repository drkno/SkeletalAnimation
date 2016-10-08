#include "NormScene.h"
#include "../../model/AssimpObject.h"

NormScene::NormScene()
{
	this->obj = new AssimpObject("objects/bvh/01_01.bvh", true);
}

NormScene::~NormScene()
{
}

void NormScene::draw(float tmp, const aiVector3D& rootPos)
{
	gluLookAt(4, 1.5, -2, rootPos.x * tmp, rootPos.y * tmp, rootPos.z * tmp, 0, 1, 0);
	glScalef(tmp, tmp, tmp);
	glTranslatef(-scene_center.x, -scene_center.y, -scene_center.z);
	obj->render();
	//stage->floor();
	//stage->cubes();
}

unsigned NormScene::getRefreshRate()
{
	return this->obj->getSecondsPerTick() * 1000;
}

void NormScene::updateTick()
{
	tick = (tick + 1) % 2130;
}
