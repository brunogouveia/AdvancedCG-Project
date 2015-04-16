#ifndef GAMEOBJECT_H__
#define GAMEOBJECT_H__ 

#include <CSCIx239.h>
#include <List>
#include <Texture.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

// #include "glm/gtc/matrix_transform.hpp"
// #include "glm/gtc/matrix_inverse.hpp"
// #include "glm/gtc/type_ptr.hpp"

class GameObject
{
protected:
    int shader;             // Shader to render this object
    int lightShader;		// Shader that render the light effect
    Texture texture;		// Texture
    glm::mat4 modelMatrix;  // Model matrix

public:
    GameObject();
    ~GameObject();
    
    virtual void init(int shader, int lightShader);
    virtual void draw(bool useLight);
    virtual void setTexture(Texture newTexture);

    // Transform functions
    virtual void translate(glm::vec3 t);
    virtual void rotate(float angle, glm::vec3 & up);
};

#endif