#pragma once
#include "../Scene.h"
#include "../../model/Tree.h"

class BoneScene : public Scene
{
public:
	BoneScene();
	~BoneScene();
	void draw(float, const aiVector3D&) override;
	unsigned getRefreshRate() override;
	void updateTick() override;
	string getTitle() override;
	Tree* tree;
	Tree* tree1;
	Tree* tree2;
	Tree* tree3;
	Tree* tree4;
	Tree* tree5;
	AssimpObject* bisonOther;
	int angle = 45;
};

