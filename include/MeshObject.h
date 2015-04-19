#ifndef MESHOBJECT_h__
#define MESHOBJECT_h__ 

#include <GameObject.h>
#include <String>

class MeshObject : public GameObject
{
private:
    unsigned int vertexArrayObj;
    unsigned int vertexBuffer;

    int numTriangles;

    float * data;
public:
    MeshObject(glm::mat4 modelMatrix);
    virtual ~MeshObject();

    virtual void init(int basicShader, int lightShader);
    virtual void shadowPass();
    virtual void rendererPass(bool useLight);

    void loadFromFile(char * fileName);
private:
};

#endif