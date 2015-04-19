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

void GameObject::draw(int shader, int depthTexture) {
    // Do nothing
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

void GameObject::translate(float tx, float ty, float tz) {
	glm::vec3 t = glm::vec3(tx, ty, tz);
	modelMatrix = glm::translate(modelMatrix, t);
}

void GameObject::rotate(float angle, glm::vec3 & up) {
	modelMatrix = glm::rotate(modelMatrix, angle, up);
}

void GameObject::scale(glm::vec3 & s) {
	modelMatrix = glm::scale(modelMatrix, s);
}

void GameObject::scale(float sx, float sy, float sz) {
	glm::vec3 s = glm::vec3(sx, sy, sz);
	modelMatrix = glm::scale(modelMatrix, s);
}
