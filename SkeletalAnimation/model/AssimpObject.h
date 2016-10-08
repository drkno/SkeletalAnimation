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
public:
	explicit AssimpObject(string filename, bool isAnimated);
	~AssimpObject();
	void update(unsigned int tick);
	void render() const;
	aiVector3D getObjectPosition() const;
	aiVector3D getSceneMin() const;
	aiVector3D getSceneMax() const;
	float getSecondsPerTick() const;
	double getAnimationDuration() const;

private:
	void loadModel(string fileName);
	void render(const aiScene* sc, const aiNode* nd) const;

	const aiScene* obj;
	vector<aiVector3D> vertices;
	vector<aiVector3D> normals;
	bool animated, bones;
	double animationDuration = 0;
	float secondsInATick = DEFAULT_TICKS_PER_SECOND;
	aiVector3D sceneMin, sceneMax;
};
