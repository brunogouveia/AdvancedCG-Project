#include "Light.h"
#include <cstring>
#include <GameWindow.h>

// Global
GLuint  Light::globalLightBuffer;
GLuint  Light::lightBuffer;
int     Light::globalLightBindingPoint = 5;
int     Light::lightBindingPoint = 3;

// Shadow
GLuint  Light::shadowBuffer;
int     Light::shadowBindingPoint = 4;
GLuint  Light::shadowFrameBuffer;
int     Light::shadowShader;
GLuint  Light::shadowTexture;

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

    updateMatrices();
}

Light::~Light() {
    // do nothing
}

void Light::activeLight() {
    // Bind local buffer
    glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);

    // Copy light's data to buffer
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(data), data);

    updateMatrices();

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

    /*********  Generate shadow buffer  **********/
    // Gen and bind buffer
    glGenBuffers(1, &shadowBuffer);
    glBindBuffer(GL_UNIFORM_BUFFER, shadowBuffer);

    // Copy global data to buffer
    float shadowData[32] = {0};
    glBufferData(GL_UNIFORM_BUFFER, sizeof(shadowData), shadowData, GL_DYNAMIC_DRAW);

    // Set binding point
    glBindBufferBase(GL_UNIFORM_BUFFER, shadowBindingPoint, shadowBuffer);

    // Unbind buffer
    glBindBuffer(GL_UNIFORM_BUFFER, 0);  

    ErrCheck("Light::init");  
}

void Light::initShadowMap(int shadowShader) {
    // unsigned int shadowtex; //  Shadow buffer texture id
    int shadowdim = 2048;
    int n;
    
    Light::shadowShader = shadowShader;
    //  Make sure multi-textures are supported
    glGetIntegerv(GL_MAX_TEXTURE_UNITS,&n);
    if (n<2) Fatal("Multiple textures not supported\n");
    
    //  Get maximum texture buffer size
    glGetIntegerv(GL_MAX_TEXTURE_SIZE,&shadowdim);
    //  Limit texture size to maximum buffer size
    glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE,&n);
    if (shadowdim>n) shadowdim = n;
    //  Limit texture size to 2048 for performance
    if (shadowdim>2048) shadowdim = 2048;
    if (shadowdim<512) Fatal("Shadow map dimensions too small %d\n",shadowdim);
    
    //  Do Shadow textures in MultiTexture 1
    glActiveTexture(GL_TEXTURE1);
    
    //  Allocate and bind shadow texture
    glGenTextures(1,&shadowTexture);
    glBindTexture(GL_TEXTURE_2D,shadowTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, shadowdim, shadowdim, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    
    //  Map single depth value to RGBA (this is called intensity)
    glTexParameteri(GL_TEXTURE_2D,GL_DEPTH_TEXTURE_MODE,GL_INTENSITY);
    
    //  Set texture mapping to clamp and linear interpolation
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    // Switch back to default textures
    glActiveTexture(GL_TEXTURE0);
    
    // Attach shadow texture to frame buffer
    glGenFramebuffers(1,&shadowFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER,shadowFrameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTexture, 0);
    //  Don't write or read to visible color buffer
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    //  Make sure this all worked
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) Fatal("Error setting up frame buffer\n");
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void Light::bindLightBuffers(int shader) {
    // Find global light block index
    int id = glGetUniformBlockIndex(shader, "GlobalLight");
    // Bind if block exists
    if (id >= 0)  {
        glUniformBlockBinding(shader, id, globalLightBindingPoint);
    }

    // Find light block index
    id = glGetUniformBlockIndex(shader, "Light");
    // Bind if block exists
    if (id >= 0) glUniformBlockBinding(shader, id, lightBindingPoint);    

    // Find Shadow block index
    id = glGetUniformBlockIndex(shader, "Shadows");
    // Bind if block exists
    if (id >= 0) glUniformBlockBinding(shader, id, shadowBindingPoint);

    ErrCheck("Light::bindLightBuffers");
}

GLuint Light::getShadowFrameBuffer() {
    return shadowFrameBuffer;
}

int Light::getShadowShader() {
    return shadowShader;
}

GLuint Light::getShadowTexture() {
    return shadowTexture;
}


void Light::updateMatrices() {
    //  Light distance
    float Dim = GameWindow::dim;
    float Ldist = sqrt(data[0]*data[0] + data[1]*data[1] + data[2]*data[2]);
    if (Ldist<1.1*Dim) Ldist = 1.1*Dim;

    // Set perspective
    projectionMatrix = glm::perspective<float>(M_PI*(114.6*atan(Dim/Ldist))/180,1,Ldist-Dim,Ldist+Dim);
    
    // Set view
    glm::vec3 lightPosition = glm::vec3(data[0],data[1],data[2]);
    if (lightPosition.x != 0.0 || lightPosition.z !=0 )
        viewMatrix = glm::lookAt(lightPosition, glm::vec3(0,0,0), glm::vec3(0,1,0));
    else 
        viewMatrix = glm::lookAt(lightPosition, glm::vec3(0,0,0), glm::vec3(0,0,1));
    glm::mat4 modelMatrix = glm::mat4(1.0);

    // Copy to buffer
    glBindBuffer(GL_UNIFORM_BUFFER, shadowBuffer);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, 16*sizeof(float), glm::value_ptr(projectionMatrix * viewMatrix * modelMatrix));

    // Bias matrix
    glm::mat4 biasMatrix(
        0.5, 0.0, 0.0, 0.0,
        0.0, 0.5, 0.0, 0.0,
        0.0, 0.0, 0.5, 0.0,
        0.5, 0.5, 0.5, 1.0
    );
    // Copy to buffer
    glBufferSubData(GL_UNIFORM_BUFFER, 16*sizeof(float), 16*sizeof(float), glm::value_ptr(biasMatrix * projectionMatrix * viewMatrix * modelMatrix));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    ErrCheck("Light::updateMatrices");
}

void Light::setPosition(float x, float y, float z) {
    // Copy to light's data
    data[0] = x;
    data[1] = y;
    data[2] = z;
    updateMatrices();
}

void Light::setPosition(float position[]) {
    // Copy to light's data
    std::memcpy(data, position, 3*sizeof(float));
    updateMatrices();
}

void Light::setDiffuse(float r, float g, float b) {
    // Copy to light's data
    data[4] = r;
    data[5] = g;
    data[6] = b;
}

void Light::setDiffuse(float diffuse[]) {
    // Copy to light's data
    std::memcpy(&(data[4]), diffuse, 3*sizeof(float));
}

void Light::setSpecular(float r, float g, float b) {
    // Copy to light's data
    data[8] = r;
    data[9] = g;
    data[10] = b;
}

void Light::setSpecular(float specular[]) {
    // Copy to light's data
    std::memcpy(&(data[8]), specular, 3*sizeof(float));
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
    updateMatrices();
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
    updateMatrices();
}

void Light::scale(glm::vec3 & s) {
    // Multiply data by s
    data[0] *= s[0];
    data[1] *= s[1];
    data[2] *= s[2];
    updateMatrices();
}