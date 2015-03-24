#include "Scene.h"

Scene::Scene() {
	// do nothing
}

Scene::~Scene() {
	// do nothing
}

void Scene::draw() {
	for (list<GameObject*>::iterator it = objects.begin(); it != objects.end(); ++it) {
		(*it)->draw();
	}
}

void Scene::addGameObject(GameObject * gameObject) {
	objects.push_back(gameObject);
}