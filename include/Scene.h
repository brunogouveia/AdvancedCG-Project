#ifndef SCENE_H__
#define SCENE_H__ 

#include <List>
#include <GameObject.h>

using namespace std;

class Scene
{
private:
	list<GameObject*> objects;
	// list<Light> lights;

public:
	Scene();
	~Scene();

	void draw();

	void addGameObject(GameObject * gameObject);
};

#endif