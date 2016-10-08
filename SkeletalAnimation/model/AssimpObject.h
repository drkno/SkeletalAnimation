#pragma once
#include <vector>
#include <assimp/scene.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <assimp/types.h>
#include <iostream>
#include <GL/freeglut.h>
#include "../core/AssimpExtensions.h"

using namespace std;

#define DEFAULT_TICKS_PER_SECOND 0.022

class AssimpObject
{
	const aiScene* obj;
	vector<aiVector3D> vertices;
	vector<aiVector3D> normals;
	bool animated;
	bool bones;
public:
	explicit AssimpObject(string filename, bool isAnimated);
	~AssimpObject();
	void update(unsigned int tick);
	void render() const;
	aiVector3D getObjectPosition() const;

	double animDuration;
	float secsPerTick = NULL;
	aiVector3D scene_min, scene_max;
private:
	void loadModel(string fileName);
	void render(const aiScene* sc, const aiNode* nd) const;
};
