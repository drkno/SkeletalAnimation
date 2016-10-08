#pragma once
#include "../Scene.h"

class BoneScene : public Scene
{
public:
	BoneScene();
	~BoneScene();
	void draw(float, const aiVector3D&) override;
	unsigned getRefreshRate() override;
	void updateTick() override;
	string getTitle() override;
};

