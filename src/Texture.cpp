#include "Texture.h"

int Texture::defaultTexture = -1;

Texture::Texture(int textureUnit, char * fileName) {
    // Set texture unit
    this->textureUnit = textureUnit;
    glActiveTexture(textureUnit);
    this->texture = LoadTexBMP(fileName);
}

Texture::~Texture() {
    // do nthing
}

void Texture::active() const {
    glActiveTexture(textureUnit);
}

int Texture::getDefaultTexture() {
    if (defaultTexture == -1) 
        defaultTexture = LoadTexBMP("textures/defaultTexture.bmp");
    return defaultTexture;
}