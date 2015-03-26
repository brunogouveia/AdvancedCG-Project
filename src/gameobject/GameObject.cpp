#include "GameObject.h"

GameObject::GameObject() {
    texture = -1;
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

void GameObject::setTexture(char * textureFileName) {
	glActiveTexture(GL_TEXTURE0);
	texture = LoadTexBMP(textureFileName);
}