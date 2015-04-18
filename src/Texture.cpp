#include "Texture.h"

int Texture::defaultTexture = -1;

Texture::Texture(int textureUnit) {
    this->textureUnit = textureUnit;
    glActiveTexture(textureUnit);
    this->texture = getDefaultTexture();
}

Texture::Texture(int textureUnit, std::string fileName) {
    // Set texture unit
    this->textureUnit = textureUnit;
    glActiveTexture(textureUnit);
    this->texture = LoadTexBMP(fileName.c_str());
}

Texture::~Texture() {
    // do nthing
}

void Texture::active() const {
    glActiveTexture(textureUnit);
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, texture);
}

int Texture::getDefaultTexture() {
    if (defaultTexture == -1) 
        defaultTexture = LoadTexBMP("textures/defaultTexture.bmp");
    return defaultTexture;
}