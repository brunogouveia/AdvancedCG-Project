#ifndef SCENE_H__
#define SCENE_H__ 

#include <List>
#include <GameObject.h>
#include <Light.h>
#include <Camera.h>
#include <Script.h>

using namespace std;

class Scene
{
public:
	Camera * camera;
	// List of objects
	list<GameObject*> objects;
	// List of lights
	list<Light*> lights;
	// List of scripts
	list<Script*> scripts;

public:
	Scene();
	~Scene();

	void shadowDraw(int shader, int depthTexture);
	void firstDraw();

	void drawObjectsWithLights(int depthFrameBuffer, int depthShader, int depthTexture);
	void updateScripts();

	void addGameObject(GameObject * gameObject);
	void addLight(Light * light);
	void addScript(Script * script);
};

#endif