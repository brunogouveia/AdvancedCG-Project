#include "Cube.h"
#include <Texture.h>
#include <iostream>

float Cube::vertices[] = {
//  X  Y  Z  W   Nx Ny Nz    s t
   //  Front
   +1,+1,+1,+1,   0, 0,+1,   1,1,
   -1,+1,+1,+1,   0, 0,+1,   0,1,
   +1,-1,+1,+1,   0, 0,+1,   1,0,
   +1,-1,+1,+1,   0, 0,+1,   1,0,
   -1,+1,+1,+1,   0, 0,+1,   0,1,
   -1,-1,+1,+1,   0, 0,+1,   0,0,
   //  Back
   +1,-1,-1,+1,   0, 0,-1,   0,0,
   -1,-1,-1,+1,   0, 0,-1,   1,0,
   -1,+1,-1,+1,   0, 0,-1,   1,1,
   +1,-1,-1,+1,   0, 0,-1,   0,0,
   -1,+1,-1,+1,   0, 0,-1,   1,1,
   +1,+1,-1,+1,   0, 0,-1,   0,1,
   //  Right
   +1,+1,+1,+1,  +1, 0, 0,   0,1,
   +1,-1,+1,+1,  +1, 0, 0,   0,0,
   +1,+1,-1,+1,  +1, 0, 0,   1,1,
   +1,+1,-1,+1,  +1, 0, 0,   1,1,
   +1,-1,+1,+1,  +1, 0, 0,   0,0,
   +1,-1,-1,+1,  +1, 0, 0,   1,0,
   //  Left
   -1,+1,+1,+1,  -1, 0, 0,   1,1,
   -1,+1,-1,+1,  -1, 0, 0,   0,1,
   -1,-1,+1,+1,  -1, 0, 0,   1,0,
   -1,-1,+1,+1,  -1, 0, 0,   1,0,
   -1,+1,-1,+1,  -1, 0, 0,   0,1,
   -1,-1,-1,+1,  -1, 0, 0,   0,0,
   //  Top
   +1,+1,+1,+1,   0,+1, 0,   1,0,
   +1,+1,-1,+1,   0,+1, 0,   1,1,
   -1,+1,+1,+1,   0,+1, 0,   0,0,
   -1,+1,+1,+1,   0,+1, 0,   0,0,
   +1,+1,-1,+1,   0,+1, 0,   1,1,
   -1,+1,-1,+1,   0,+1, 0,   0,1,
   //  Bottom
   -1,-1,-1,+1,   0,-1, 0,   0,0,
   +1,-1,-1,+1,   0,-1, 0,   1,0,
   -1,-1,+1,+1,   0,-1, 0,   0,1,
   -1,-1,+1,+1,   0,-1, 0,   0,1,
   +1,-1,-1,+1,   0,-1, 0,   1,0,
   +1,-1,+1,+1,   0,-1, 0,   1,1
};


Cube::Cube(glm::mat4 modelMatrix) : GameObject() {
    this->modelMatrix = modelMatrix;
}

Cube::~Cube() {
    // do nothing
}

void Cube::init(int shader, int lightShader) {
    // Set cube's shader
    this->shader = shader;
    this->lightShader = lightShader;

    // Create vertex array object
    glGenVertexArrays(1, &vertexArrayObj);
    glBindVertexArray(vertexArrayObj);

    // Create buffer
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    // Copy data to 
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Unbind vao and buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Cube::shadowPass() {
    // Set program
    glUseProgram(Light::getShadowShader());

    // Set model matrix
    GameWindow::getRenderer()->camera->setModelMatrix(modelMatrix);

    // Bind vao and buffer
    glBindVertexArray(vertexArrayObj);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    // Set attribute 0 - vertex (vec4)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 9*sizeof(float), (void*)0);

    // Draw cube
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Disable attributes
    glDisableVertexAttribArray(0);

    // Unbind everything
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);

    // Unbind shader
    glUseProgram(0);
}

void Cube::rendererPass(bool useLight) {
    // Set program
    glUseProgram(useLight ? lightShader : shader);

    // Set model matrix
    GameWindow::getRenderer()->camera->setModelMatrix(modelMatrix);

    // Bind vao and buffer
    glBindVertexArray(vertexArrayObj);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    // Set attribute 0 - vertex (vec4)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 9*sizeof(float), (void*)0);
    // Set attribute 1 - normal (vec3)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (void*)(4*sizeof(float)));
    // Set attribute 2 - texture (vec2)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9*sizeof(float), (void*)(7*sizeof(float)));

    // Activate texture
    texture.active();
    // Bind texture
    texture.bind();
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, Light::getShadowTexture());
    glTexGendv(GL_S,GL_EYE_PLANE,Light::Svec);
    glTexGendv(GL_T,GL_EYE_PLANE,Light::Tvec);
    glTexGendv(GL_R,GL_EYE_PLANE,Light::Rvec);
    glTexGendv(GL_Q,GL_EYE_PLANE,Light::Qvec);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_MODE,GL_COMPARE_R_TO_TEXTURE);

    // Activate normalMap
    normalMap.active();
    // Bind normal Map
    normalMap.bind();

    // Set uniform value
    int id = glGetUniformLocation(useLight ? lightShader : shader, "text");
    if (id >= 0) glUniform1i(id, 0);
    id = glGetUniformLocation(useLight ? lightShader : shader, "depthText");
    if (id >= 0) glUniform1i(id, 1);
    id = glGetUniformLocation(useLight ? lightShader : shader, "normalMap");
    if (id >= 0) glUniform1i(id, 2);

    // Active material
    material->activeMaterial();

    // Draw cube
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Disable attributes
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    // Unbind everything
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glUseProgram(0);
}