#ifndef GAMEOBJECT_H__
#define GAMEOBJECT_H__ 

#include <CSCIx239.h>
#include <List>
#include <Texture.h>
#include <Transform.h>

class GameObject : public Transform
{
protected:
    int shader;             // Shader to render this object
    int lightShader;		// Shader that render the light effect
    Texture texture;		// Texture
    Texture normalMap;      // Normal map
    glm::mat4 modelMatrix;  // Model matrix

public:
    GameObject();
    virtual ~GameObject();
    
    virtual void init(int shader, int lightShader);
    virtual void shadowPass();
    virtual void rendererPass(bool useLight);
    virtual void setTexture(Texture newTexture);
    virtual void setNormalMap(Texture newNormalMap);

    // Transform functions
    virtual void translate(glm::vec3 & t);
    virtual void translate(float tx, float ty, float tz);
    virtual void rotate(float angle, glm::vec3 & up);
    virtual void scale(glm::vec3 & s);
    virtual void scale(float sx, float sy, float sz);
};

#endif