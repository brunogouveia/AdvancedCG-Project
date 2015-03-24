#ifndef CUBE_H__
#define CUBE_H__ 

#include "CSCIx239.h"
#include <glm/glm.hpp>
#include <GameObject.h>

class Cube: public GameObject
{
private:
    unsigned int vertexArrayObj;
    unsigned int vertexBuffer;
    
    static float vertices[];

public:
    Cube();
    ~Cube();
    
    virtual void init(int shader);
    virtual void draw();
};

#endif