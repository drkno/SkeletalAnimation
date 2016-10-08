#include <iostream>
#include "core/OpenGLManager.h"
#include "core/scenes/NormScene.h"
#include "core/scenes/BoneScene.h"

using namespace std;

// Main entry point for the application
int main(int argc, char* argv[]) {
	Scene* scene;

	if (argc > 2)
	{
		cerr << "You should provide a mode to run using.\nModes are:\n\t- norm [default]\n\t- bone" << endl;
		return -1;
	}

	if (argc == 2 && string(argv[1]) == "bone")
	{
		scene = new BoneScene();
	}
	else
	{
		scene = new NormScene();
	}

	// Start the GUI.
	// Note: this should never, ever, ever be called multiple times...
	OpenGLManager::run(argc, argv, scene);
	return 0;
}