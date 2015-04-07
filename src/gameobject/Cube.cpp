#include "Cube.h"
#include <Texture.h>
#include <iostream>

float Cube::vertices[] = {
//  X  Y  Z  W   Nx Ny Nz    R G B   s t
   //  Front
   +1,+1,+1,+1,   0, 0,+1,   1,0,0,  1,1,
   -1,+1,+1,+1,   0, 0,+1,   1,0,0,  0,1,
   +1,-1,+1,+1,   0, 0,+1,   1,0,0,  1,0,
   +1,-1,+1,+1,   0, 0,+1,   1,0,0,  1,0,
   -1,+1,+1,+1,   0, 0,+1,   1,0,0,  0,1,
   -1,-1,+1,+1,   0, 0,+1,   1,0,0,  0,0,
   //  Back
   +1,-1,-1,+1,   0, 0,-1,   0,0,1,  0,0,
   -1,-1,-1,+1,   0, 0,-1,   0,0,1,  1,0,
   -1,+1,-1,+1,   0, 0,-1,   0,0,1,  1,1,
   +1,-1,-1,+1,   0, 0,-1,   0,0,1,  0,0,
   -1,+1,-1,+1,   0, 0,-1,   0,0,1,  1,1,
   +1,+1,-1,+1,   0, 0,-1,   0,0,1,  0,1,
   //  Right
   +1,+1,+1,+1,  +1, 0, 0,   1,1,0,  0,1,
   +1,-1,+1,+1,  +1, 0, 0,   1,1,0,  0,0,
   +1,+1,-1,+1,  +1, 0, 0,   1,1,0,  1,1,
   +1,+1,-1,+1,  +1, 0, 0,   1,1,0,  1,1,
   +1,-1,+1,+1,  +1, 0, 0,   1,1,0,  0,0,
   +1,-1,-1,+1,  +1, 0, 0,   1,1,0,  1,0,
   //  Left
   -1,+1,+1,+1,  -1, 0, 0,   0,1,0,  1,1,
   -1,+1,-1,+1,  -1, 0, 0,   0,1,0,  0,1,
   -1,-1,+1,+1,  -1, 0, 0,   0,1,0,  1,0,
   -1,-1,+1,+1,  -1, 0, 0,   0,1,0,  1,0,
   -1,+1,-1,+1,  -1, 0, 0,   0,1,0,  0,1,
   -1,-1,-1,+1,  -1, 0, 0,   0,1,0,  0,0,
   //  Top
   +1,+1,+1,+1,   0,+1, 0,   0,1,1,  1,0,
   +1,+1,-1,+1,   0,+1, 0,   0,1,1,  1,1,
   -1,+1,+1,+1,   0,+1, 0,   0,1,1,  0,0,
   -1,+1,+1,+1,   0,+1, 0,   0,1,1,  0,0,
   +1,+1,-1,+1,   0,+1, 0,   0,1,1,  1,1,
   -1,+1,-1,+1,   0,+1, 0,   0,1,1,  0,1,
   //  Bottom
   -1,-1,-1,+1,   0,-1, 0,   1,0,1,  0,0,
   +1,-1,-1,+1,   0,-1, 0,   1,0,1,  1,0,
   -1,-1,+1,+1,   0,-1, 0,   1,0,1,  0,1,
   -1,-1,+1,+1,   0,-1, 0,   1,0,1,  0,1,
   +1,-1,-1,+1,   0,-1, 0,   1,0,1,  1,0,
   +1,-1,+1,+1,   0,-1, 0,   1,0,1,  1,1
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

void Cube::draw(bool useLight) {
    // Set program
    glUseProgram(useLight ? lightShader : shader);

    // Set model matrix
    GameWindow::getRenderer()->camera->setModelMatrix(modelMatrix);

    // Bind vao and buffer
    glBindVertexArray(vertexArrayObj);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    // Set attribute 0 - vertex (vec4)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 12*sizeof(float), (void*)0);
    // Set attribute 1 - normal (vec3)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12*sizeof(float), (void*)(4*sizeof(float)));
    // Set attribute 2 - color (vec3)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 12*sizeof(float), (void*)(7*sizeof(float)));
    // Set attribute 3 - texture (vec2)
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 12*sizeof(float), (void*)(10*sizeof(float)));   

    // Activate texture
    // texture.active();
    glActiveTexture(GL_TEXTURE0);
    // Bind texture
    texture.bind();
    // if (texture >= 0) {
    //     glBindTexture(GL_TEXTURE_2D, texture);
    // } else {
    //     glBindTexture(GL_TEXTURE_2D, Texture::getDefaultTexture());
    // }
    // Set uniform value
    int id = glGetUniformLocation(shader, "text");
    if (id >= 0) glUniform1i(id, 0);

    // Draw cube
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Disable attributes
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);

    // Unbind everything
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glUseProgram(0);
}