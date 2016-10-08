#include <vector>
#include "OpenGLManager.h"

vector<Scene*> windowScenes;

void OpenGLManager::run(int argc, char* argv[], vector<Scene*> scenes)
{
	windowScenes = scenes;
	glutInit(&argc, argv);

	auto totalx = WINDOW_XPOS;

	for (auto i = 0; i < scenes.size(); i++)
	{
		auto windowScene = scenes[i];
		glutInitWindowPosition(totalx, WINDOW_YPOS);
		glutInitWindowSize(windowScene->getWidth(), windowScene->getHeight());
		totalx += windowScene->getWidth() + WINDOW_XPOS;
		auto window = glutCreateWindow(windowScene->getTitle().c_str());
		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
		glutSetWindowData(reinterpret_cast<void*>(window));
		windowScene->setWindow(window);

		glutDisplayFunc(windowDisplayCallback);
		glutTimerFunc(windowScene->getRefreshRate(), displayRefreshCallback, i);
		glutReshapeFunc(windowReshapeCallback);
		glutSpecialFunc(specialKeypressCallback);
		glutMouseFunc(mouseClickCallback);
		glutKeyboardFunc(keypressCallback);
		windowScene->initialise();
	}
	glutInitContextVersion(4, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	
	
	glutMainLoop();
	for (auto i = 0; i < windowScenes.size(); i++)
	{
		delete windowScenes[i];
	}
}

void OpenGLManager::windowDisplayCallback()
{
	for (auto i = 0; i < windowScenes.size(); i++)
	{
		windowScenes[i]->display();
	}
}

void OpenGLManager::displayRefreshCallback(int value)
{
	windowScenes[value]->update();
	glutTimerFunc(windowScenes[value]->getRefreshRate(), displayRefreshCallback, value);
}

void OpenGLManager::windowReshapeCallback(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
}

void OpenGLManager::specialKeypressCallback(int key, int x, int y)
{
	for (auto i = 0; i < windowScenes.size(); i++)
	{
		windowScenes[i]->special(key, x, y);
	}
	if (key == GLUT_KEY_F12)
	{
		glutFullScreenToggle();
	}
}

void OpenGLManager::keypressCallback(unsigned char key, int x, int y)
{
	for (auto i = 0; i < windowScenes.size(); i++)
	{
		windowScenes[i]->key(key, x, y);
	}
}

void OpenGLManager::mouseClickCallback(int button, int state, int x, int y)
{
	if (state != GLUT_UP) return;
	for (auto i = 0; i < windowScenes.size(); i++)
	{
		windowScenes[i]->key(-1, x, y);
	}
}

