#include "GameObject.h"

GameObject::GameObject() : texture(GL_TEXTURE0), normalMap(GL_TEXTURE2, true) {
	// do nothing
}

GameObject::~GameObject() {
    // Do nothing
}

void GameObject::init(int shader, int lightShader) {
    this->shader = shader;
    this->lightShader = lightShader;
}

void GameObject::shadowPass() {
    // Do nothing
}

void GameObject::rendererPass(bool useLight) {
    // Do nothing
}

void GameObject::setTexture(Texture newTexture) {
	texture = newTexture;
}

void GameObject::setNormalMap(Texture newNormalMap) {
	normalMap = newNormalMap;
}

void GameObject::translate(glm::vec3 & t) {
	glm::mat4 transf = glm::translate(glm::mat4(1.0), t);
	modelMatrix = transf * modelMatrix;
}

void GameObject::translate(float tx, float ty, float tz) {
	glm::vec3 t = glm::vec3(tx, ty, tz);
	glm::mat4 transf = glm::translate(glm::mat4(1.0), t);
	modelMatrix = transf * modelMatrix;
}

void GameObject::rotate(float angle, glm::vec3 & up) {
	glm::mat4 transf = glm::rotate(glm::mat4(1.0), angle, up);
	modelMatrix = transf * modelMatrix;
}

void GameObject::scale(glm::vec3 & s) {
	glm::mat4 transf = glm::scale(glm::mat4(1.0), s);
	modelMatrix = transf * modelMatrix;
}

void GameObject::scale(float sx, float sy, float sz) {
	glm::vec3 s = glm::vec3(sx, sy, sz);
	glm::mat4 transf = glm::scale(glm::mat4(1.0), s);
	modelMatrix = transf * modelMatrix;
}
