#pragma once

#include <math.h>
#include "SceneObject.h"

#define BACKGROUND_COLOUR 0.0, 0.0, 0.0, 0.0
#define PERSPECTIVE_PROJECTION 60., 1.0, 10.0, 1000.0
#define HALF_WIDTH 300
#define HALF_DEPTH 200

class Stage : public SceneObject
{
public:
	Stage();
	void setAngle(int);
	void changeCameraViewAngle(int);

private:
	int cameraViewAngle;
	int cameraAngle;
	void floor();
	void draw();
	void drawCamera();
};

