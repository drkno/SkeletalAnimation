#pragma once
#include <string>
#include <vector>
#include "../model/AssimpObject.h"
#include "../model/Floor.h"

using namespace std;

class Scene
{
protected:
	Scene();

	AssimpObject* obj;
	Floor* floor;
	unsigned int tick;
	aiVector3D scene_center;
	int window;
	virtual void updateTick();
	virtual void draw(float, const aiVector3D&);

public:
	virtual ~Scene();
	void display();
	virtual void special(int, int, int) const;
	virtual void key(unsigned char, int, int) const;
	int getWidth() const;
	int getHeight() const;
	virtual string getTitle();
	virtual unsigned getRefreshRate();
	void initialise() const;
	void update();
	void setWindow(int window);
};

