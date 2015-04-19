#ifndef CUBE_H__
#define CUBE_H__ 

#include <CSCIx239.h>
#include <glm/glm.hpp>
#include <GameWindow.h>
#include <GameObject.h>

class Cube: public GameObject
{
private:
    unsigned int vertexArrayObj;
    unsigned int vertexBuffer;
    
    static float vertices[];

public:
    Cube(glm::mat4 modelMatrix);
    virtual ~Cube();
    
    virtual void init(int shader, int lightShader);
    virtual void shadowPass();
    virtual void rendererPass(bool useLight);
};

#endif