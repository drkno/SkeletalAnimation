#include <vector>
#include "OpenGLManager.h"

Scene* OpenGLManager::windowScene = nullptr;
double refreshRate;

void OpenGLManager::run(int argc, char* argv[], Scene* scene)
{
	windowScene = scene;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(windowScene->getWidth(), windowScene->getHeight());
	glutInitWindowPosition(WINDOW_XPOS, WINDOW_YPOS);
	glutCreateWindow(windowScene->getTitle().c_str());
	glutInitContextVersion(4, 2);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutDisplayFunc(windowDisplayCallback);
	refreshRate = windowScene->getRefreshRate();
	glutTimerFunc(refreshRate, displayRefreshCallback, 0);
	glutReshapeFunc(windowReshapeCallback);
	glutSpecialFunc(specialKeypressCallback);
	glutMouseFunc(mouseClickCallback);
	glutKeyboardFunc(keypressCallback);
	windowScene->initialise();
	glutMainLoop();
	windowScene->~Scene();
}

void OpenGLManager::windowDisplayCallback()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	windowScene->display();
}

void OpenGLManager::displayRefreshCallback(int value)
{
	windowScene->update();
	glutTimerFunc(refreshRate, displayRefreshCallback, ++value);
}

void OpenGLManager::windowReshapeCallback(GLint newWidth, GLint newHeight)
{
	glViewport(0, 0, newWidth, newHeight);
}

void OpenGLManager::specialKeypressCallback(int key, int x, int y)
{
	windowScene->special(key, x, y);
	if (key == GLUT_KEY_F12)
	{
		glutFullScreenToggle();
	}
}

void OpenGLManager::keypressCallback(unsigned char key, int x, int y)
{
	windowScene->key(key, x, y);
}

void OpenGLManager::mouseClickCallback(int button, int state, int x, int y)
{
	if (state == GLUT_UP) windowScene->special(-1, x, y);
}

