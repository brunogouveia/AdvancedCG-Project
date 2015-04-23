#include <scripts/LightScript.h>
#include <Input.h>

void LightScript::OnUpdate() {
	static bool isPressed = false;
	glm::vec3 up = glm::vec3(0, 1, 0);
	if (Input::getKeyUp('h')) {
		isPressed = !isPressed;
		printf("Pressed");
	}

	if (isPressed)
		transform->rotate(0.01, up);
}
