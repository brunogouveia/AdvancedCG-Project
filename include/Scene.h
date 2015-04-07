#ifndef SCENE_H__
#define SCENE_H__ 

#include <List>
#include <GameObject.h>
#include <Light.h>
#include <Camera.h>

using namespace std;

class Scene
{
private:
	Camera * camera;
	list<GameObject*> objects;
	list<Light*> lights;

public:
	Scene();
	~Scene();

	void firstDraw();

	void drawObjectsWithLights();

	void addGameObject(GameObject * gameObject);
	void addLight(Light * light);
};

#endif