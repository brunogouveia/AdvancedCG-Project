#ifndef SCRIPT_H__
#define SCRIPT_H__ 

#include <glm/glm.hpp>
#include <GameObject.h>

class Script
{
protected:
	// GameObject Reference
	GameObject * gameObject;

public:
	Script(GameObject * gameObject);
	~Script();

	virtual void OnStart();
	virtual void OnUpdate();
};

#endif