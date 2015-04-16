#include "Light.h"
#include <cstring>

GLuint Light::globalLightBuffer;
GLuint Light::lightBuffer;
int Light::globalLightBindingPoint = 5;
int Light::lightBindingPoint = 3;

Light::Light() {
    // do nothing
    float newData[12] = {
        // All attributes should be treated as a vec3 with last component equal 1.0 (except Specular)
        // Position
        0.0, 0.0, 0.0, 1.0,
        // Diffuse
        1.0, 1.0, 1.0, 1.0,
        // Specular - shininess (shininess is the last float of Specular)
        1.0, 1.0, 1.0, 1.0
    };

    // Copy to light's data
    std::memcpy(data, newData, sizeof(data));
}

Light::~Light() {
    // do nothing
}

void Light::activeLight() {
    // Bind local buffer
    glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);

    // Copy light's data to buffer
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(data), data);

    // Unbind buffer
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Light::init() {
    /*********  Generate global buffer  **********/
    // Gen and bind buffer
    glGenBuffers(1, &globalLightBuffer);
    glBindBuffer(GL_UNIFORM_BUFFER, globalLightBuffer);

    // Copy global data to buffer
    float globalData[] = {
        // Global light
        0.1, 0.1, 0.1, 1.0,
        // Ambient light
        0.1, 0.1, 0.1, 1.0
    };
    glBufferData(GL_UNIFORM_BUFFER, sizeof(globalData), globalData, GL_DYNAMIC_DRAW);

    // Set binding point
    glBindBufferBase(GL_UNIFORM_BUFFER, globalLightBindingPoint, globalLightBuffer);

    /*********  Generate local light buffer  **********/
    // Gen and bind buffer
    glGenBuffers(1, &lightBuffer);
    glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);

    // Initialize buffer
    float newData[12] = {
        // All attributes should be treated as a vec3 with last component equal 1.0 (except Specular)
        // Position
        0.0, 0.0, 0.0, 1.0,
        // Diffuse
        1.0, 1.0, 1.0, 1.0,
        // Specular - shininess (shininess is the last float of Specular)
        1.0, 1.0, 1.0, 1.0
    };
    glBufferData(GL_UNIFORM_BUFFER, sizeof(newData), newData, GL_DYNAMIC_DRAW);

    // Set binding point
    glBindBufferBase(GL_UNIFORM_BUFFER, lightBindingPoint, lightBuffer);

    // Unbind buffer
    glBindBuffer(GL_UNIFORM_BUFFER, 0);    
}

void Light::bindLightBuffers(int shader) {
    // Find global light block index
    int id = glGetUniformBlockIndex(shader, "GlobalLight");
    // Bind if block exists
    if (id >= 0)  {
        glUniformBlockBinding(shader, id, globalLightBindingPoint);
        printf("foi\n");
    }

    // Find light block index
    id = glGetUniformBlockIndex(shader, "Light");
    // Bind if block exists
    if (id >= 0) glUniformBlockBinding(shader, id, lightBindingPoint);    
}

void Light::setPosition(float x, float y, float z) {
    // Copy to light's data
    data[0] = x;
    data[1] = y;
    data[2] = z;
}

void Light::setPosition(float position[]) {
    // Copy to light's data
    std::memcpy(data, position, sizeof(position));
}

void Light::setDiffuse(float r, float g, float b) {
    // Copy to light's data
    data[4] = r;
    data[5] = g;
    data[6] = b;
}

void Light::setDiffuse(float diffuse[]) {
    // Copy to light's data
    std::memcpy(&(data[4]), diffuse, sizeof(diffuse));
}

void Light::setSpecular(float r, float g, float b) {
    // Copy to light's data
    data[8] = r;
    data[9] = g;
    data[10] = b;
}

void Light::setSpecular(float specular[]) {
    // Copy to light's data
    std::memcpy(&(data[8]), specular, sizeof(specular));
}

void Light::setShininess(float shininess) {
    // Set shininess
    data[11] = shininess;
}

void Light::translate(glm::vec3 & t){
    // Add t to data
    data[0] += t[0];
    data[1] += t[1];
    data[2] += t[2];
};

void Light::rotate(float rad, glm::vec3 & normal) {
    // Create position
    glm::vec3 p = glm::vec3(data[0], data[1], data[2]);

    // Rotate point
    p = glm::rotate(p, rad, normal);

    // Copy back do data
    data[0] = p[0];
    data[1] = p[1];
    data[2] = p[2];
}

void Light::scale(glm::vec3 & s) {
    // Multiply data by s
    data[0] *= s[0];
    data[1] *= s[1];
    data[2] *= s[2];
}