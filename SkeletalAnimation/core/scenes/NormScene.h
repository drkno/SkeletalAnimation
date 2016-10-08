#pragma once
#include "../Scene.h"
#include "../../model/AssimpObject.h"

class NormScene : public Scene
{
public:
	NormScene();
	~NormScene();
	void draw(float, const aiVector3D&) override;
	unsigned getRefreshRate() override;
	void updateTick() override;
	string getTitle() override;
};
