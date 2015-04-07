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