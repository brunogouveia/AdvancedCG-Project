#include <scripts/LightScript.h>

void LightScript::OnUpdate() {
	glm::vec3 up = glm::vec3(0,1,0);
	transform->rotate(0.005, up);
}