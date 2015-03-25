#ifndef RENDERER_H__
#define RENDERER_H__ 

#include "CSCIx239.h"
#include <Scene.h>
#include <Camera.h>

class Renderer
{
public:
    Scene * scene;  //  Scene to be rendered
    Camera * camera;//  Camera info
    int th;         //  Azimuth of view angle
    int ph;         //  Elevation of view angle
    int zh;         //  Light angle
    float fov;      //  Field of view (angles)
    float asp;      //  Screen aspect ratio
    float dim;      //  World dimension

    Renderer(Scene * s);
    ~Renderer();

    int createShaderProg(char * vertShaderFile, char * fragShaderFile);

    void setScene(Scene * s);
    void setCamera(Camera * c);

    // Callback functions
    void display();
    void reshape(int width, int height);
    void special(int key,int x,int y);
    void key(unsigned char ch,int x,int y);
    void idle(); 
};

#endif