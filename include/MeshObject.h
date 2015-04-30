#ifndef MESHOBJECT_h__
#define MESHOBJECT_h__ 

#include <GameObject.h>
#include <string>
#include <vector>

class MeshObject : public GameObject
{
private:
    unsigned int vertexArrayObj;
    unsigned int vertexBuffer;

    int numVertices;

public:
    MeshObject(glm::mat4 modelMatrix);
    virtual ~MeshObject();

    /**
     *  This method initialize the vao and buffers, so it
     * must be the first method to be called. However it must
     * be called after OpenGL initialization.
     *  You can also choose the basic shader and light shader
     * that will render the MeshObject.
     */
    virtual void init(int basicShader, int lightShader);

    /**
     *  This method is called to compute the shadow mapping, 
     * which means that the color is useless. Hence, you shouldn't use
     * any attribute such as textures or normals, just the vertex.
     */
    virtual void shadowPass();

    /**
     *  This method is called when the renderer wants to draw 
     * the cube using color. Hence, attributes such as textures
     * and normals are important here.
     */
    virtual void rendererPass(bool useLight);

    virtual void deferredPass();

    /**
     *  This method loads the triangle mesh from a file. If you don't
     * call this methods, this mesh is gonna be useless.
     */
    void loadFromFile(char * fileName);
private:
};

#endif
