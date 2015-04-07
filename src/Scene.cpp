#include "Scene.h"

Scene::Scene() {
	// Initialize camera
	camera = new Camera();
}

Scene::~Scene() {
	// do nothing
}

void Scene::firstDraw() {
	// For each object, draw it with regular shader
	for (list<GameObject*>::iterator curObject = objects.begin(); curObject != objects.end(); ++curObject) {
		(*curObject)->draw(false);
	}
}

void Scene::drawObjectsWithLights() {
	// For each light
	for (list<Light*>::iterator curLight = lights.begin(); curLight != lights.end(); curLight++) {
		// Active current light
		(*curLight)->activeLight();
		
		// Draw objects with current light
		for (list<GameObject*>::iterator curObject = objects.begin(); curObject != objects.end(); ++curObject) {
			(*curObject)->draw(true);
		}
	}
}

void Scene::addGameObject(GameObject * gameObject) {
	objects.push_back(gameObject);
}

void Scene::addLight(Light * light) {
	lights.push_back(light);
}