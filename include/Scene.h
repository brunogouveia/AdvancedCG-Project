#ifndef SCENE_H__
#define SCENE_H__ 

#include <List>
#include <GameObject.h>
#include <Camera.h>

using namespace std;

class Scene
{
private:
	Camera * camera;
	list<GameObject*> objects;
	// list<Light> lights;

public:
	Scene();
	~Scene();

	void draw();

	void addGameObject(GameObject * gameObject);
};

#endif