#include "GameObject.h"

GameObject::GameObject() : texture(GL_TEXTURE0) {
	// do nothing
}

GameObject::~GameObject() {
    // Do nothing
}

void GameObject::init(int shader, int lightShader) {
    this->shader = shader;
    this->lightShader = lightShader;
}

void GameObject::draw(bool useLight) {
    // Do nothing
}

void GameObject::setTexture(Texture newTexture) {
	texture = newTexture;
}

void GameObject::translate(glm::vec3 & t) {
	modelMatrix = glm::translate(modelMatrix, t);
}

void GameObject::rotate(float angle, glm::vec3 & up) {
	modelMatrix = glm::rotate(modelMatrix, angle, up);
}