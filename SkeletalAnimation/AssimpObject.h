#pragma once
#include <vector>
#include <assimp/scene.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>
#include <assimp/types.h>
#include <iostream>
#include <GL/freeglut.h>
#include "AssimpExtensions.h"

class AssimpObject
{
	const aiScene* model;
	vector<aiVector3D> vertices = vector<aiVector3D>();
	vector<aiVector3D> normals = vector<aiVector3D>();
	bool isAnimation;
	bool useBones;
public:
	explicit AssimpObject(const char* filename, bool isAnimation = true, bool useBones = false);
	void setSecsPerTick(float secsPerTick);
	void updateAnimation(unsigned int tick);
	void render() const;
	void releaseResources() const;
	aiVector3D getRootPos();

	double animDuration;
	float secsPerTick = NULL;
	aiVector3D scene_min, scene_max;
private:
	const aiScene* loadModel(const char* fileName);
	void updateVerts();
	void render(const aiScene* sc, const aiNode* nd) const;
};
