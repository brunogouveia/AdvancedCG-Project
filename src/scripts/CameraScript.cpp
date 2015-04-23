/*
 * CameraScript.cpp
 *
 *  Created on: Apr, 2015
 *      Author: Bruno
 */

#include "scripts/CameraScript.h"
#include <Input.h>

void CameraScript::OnUpdate() {
	// Get the camera
	camera = dynamic_cast<Camera*>(transform);
	if (camera != NULL) {
		// Rotate camera
		if (Input::getKey(KeyCode::UpArrow)) {
			glm::vec3 normal = glm::vec3(1, 0, 0);
			camera->localRotate(-0.1, normal);
		}
		if (Input::getKey(KeyCode::DownArrow)) {
			glm::vec3 normal = glm::vec3(1, 0, 0);
			camera->localRotate(0.1, normal);
		}
		if (Input::getKey(KeyCode::RightArrow)) {
			glm::vec3 normal = camera->toTransformVec(glm::vec3(0, 1, 0));
			camera->localRotate(-0.1, normal);
		}
		if (Input::getKey(KeyCode::LeftArrow)) {
			glm::vec3 normal = camera->toTransformVec(glm::vec3(0, 1, 0));
			camera->localRotate(0.1, normal);
		}

		// Move camera
		if (Input::getKey('q') || Input::getKey('Q')) {
			camera->moveUp(0.1);
		}
		if (Input::getKey('z') || Input::getKey('Z')) {
			camera->moveDown(0.1);
		}
		if (Input::getKey('w') || Input::getKey('W')) {
			camera->moveForward(0.1);
		}
		if (Input::getKey('s') || Input::getKey('S')) {
			camera->moveBackward(0.1);
		}
		if (Input::getKey('a') || Input::getKey('A')) {
			camera->moveLeft(0.1);
		}
		if (Input::getKey('d') || Input::getKey('D')) {
			camera->moveRight(0.1);
		}

		// Zoom
		if (Input::getKey(KeyCode::PageDown))
			camera->zoomOut();
		else if (Input::getKey(KeyCode::PageUp))
			camera->zoomIn();
	} else {
		printf("NUll");
	}

	// Check if user wants to close
	if (Input::getKey(KeyCode::Esc))
		exit(0);
}
