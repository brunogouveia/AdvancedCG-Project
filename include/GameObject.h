#ifndef GAMEOBJECT_H__
#define GAMEOBJECT_H__ 

#include <CSCIx239.h>
#include <Texture.h>
#include <glm/glm.hpp>
// #include "glm/gtc/matrix_transform.hpp"
// #include "glm/gtc/matrix_inverse.hpp"
// #include "glm/gtc/type_ptr.hpp"

class GameObject
{
protected:
    int shader;             // Shader to render this object
    Texture texture;			// Texture
    glm::mat4 modelMatrix;  // Model matrix

public:
    GameObject();
    ~GameObject();
    
    virtual void init(int shader);
    virtual void draw();
    virtual void setTexture(Texture newTexture);
};

#endif