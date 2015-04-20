#ifndef LIGHT_H__
#define LIGHT_H__ 

#include <CSCIx239.h>
#include <Transform.h>

class Light: public Transform
{
private:
    // Static attributes, this is global to all lights
    static GLuint globalLightBuffer;
    static GLuint lightBuffer; // This is shared between all lights.
    static int globalLightBindingPoint;
    static int lightBindingPoint;

    // Shadow
    static GLuint shadowBuffer;
    static int shadowBindingPoint;
    static GLuint shadowFrameBuffer;
    static int shadowShader;
    static GLuint shadowTexture;


    // Light attributes
    float data[12];

public:
    // Shadow texture planes
    static double Svec[4];   // Texture planes S
    static double Tvec[4];   // Texture planes T
    static double Rvec[4];   // Texture planes R
    static double Qvec[4];   // Texture planes Q
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    Light();
    ~Light();

    // Set methods
    void setPosition(float x, float y, float z);
    void setPosition(float position[]);
    void setDiffuse(float r, float g, float b);
    void setDiffuse(float diffuse[]);
    void setSpecular(float r, float g, float b);
    void setSpecular(float specular[]);
    void setShininess(float shininess);


    void activeLight();
    void updateMatrices();

    static void init();
    static void initShadowMap(int shadowShader);
    static void bindLightBuffers(int shader);

    static GLuint getShadowFrameBuffer();
    static int getShadowShader();
    static GLuint getShadowTexture();

    // Tranform overwrite
    virtual void translate(glm::vec3 & t);
    virtual void rotate(float rad, glm::vec3 & normal);
    virtual void scale(glm::vec3 & s);
    
};

#endif