#include "GameObject.h"

GameObject::GameObject() : texture(GL_TEXTURE0) {
	// do nothing
}

GameObject::~GameObject() {
    // Do nothing
}

void GameObject::init(int shader) {
    this->shader = shader;
}

void GameObject::draw() {
    // Do nothing
}

void GameObject::setTexture(Texture newTexture) {
	texture = newTexture;
}