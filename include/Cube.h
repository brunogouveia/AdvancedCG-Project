#ifndef CUBE_H__
#define CUBE_H__ 

#include <CSCIx239.h>
#include <glm/glm.hpp>
#include <GameWindow.h>
#include <GameObject.h>

class Cube: public GameObject
{
private:
	glm::mat4 modelMatrix;
    unsigned int vertexArrayObj;
    unsigned int vertexBuffer;
    
    static float vertices[];

public:
    Cube(glm::mat4 modelMatrix);
    ~Cube();
    
    virtual void init(int shader);
    virtual void draw();
};

#endif