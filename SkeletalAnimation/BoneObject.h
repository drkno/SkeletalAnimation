#pragma once

#include <string>
#include <iostream>
#include <GL/freeglut.h>
#include "Stage.h"
#include <vector>
#include <assimp/cimport.h>
#include <assimp/types.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "assimp_extras.h"
#include <functional>

using namespace std;

class BoneObject
{
public:
	void loadModel(string fileName, bool isAnimation);
	void update(int value);
	BoneObject(string file);
	~BoneObject();

	double secsPerTick;
	vector<aiVector3D> vertices;
	vector<aiVector3D> normals;
	aiVector3D scene_min, scene_max, scene_center;
	const aiScene* object;
};

