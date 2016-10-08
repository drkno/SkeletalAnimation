#include <iostream>
#include <vector>
#include "core/OpenGLManager.h"
#include "core/scenes/NormScene.h"
#include "core/scenes/BoneScene.h"

using namespace std;

int main(int argc, char* argv[]) {
	vector<Scene*> scenes;
	scenes.push_back(new NormScene());
	scenes.push_back(new BoneScene());

	OpenGLManager::run(argc, argv, scenes);
	return 0;
}