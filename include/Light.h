#ifndef LIGHT_H__
#define LIGHT_H__ 

#include <CSCIx239.h>

class Light
{
private:
    // Static attributes, this is global to all lights
    static GLuint globalLightBuffer;
    static GLuint lightBuffer; // This is shared between all lights.
    static int globalLightBindingPoint;
    static int lightBindingPoint;

    // Light attributes
    float data[12];

public:
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

    static void init();
    static void bindLightBuffers(int shader);
    
};

#endif