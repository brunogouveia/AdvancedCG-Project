#ifndef CAMERA_H__
#define CAMERA_H__ 

#include "CSCIx239.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"

class Camera
{
private:
    // Private data
    const int projectionBindingPoint;
    unsigned int projectionBuffer;
    // Matrices
    glm::mat4 projectionMatrix;
    glm::mat4 modelViewMatrix;
    glm::mat4 normalMatrix;
    glm::mat4 MVPMatrix;

public:
    Camera();
    ~Camera();

    void bindProjectionBuffer(int shader);
    
    void setPerspective(float fov, float aspRatio, float zNear, float zFar);

    void lookAt(float ex, float ey, float ez, float cx, float cy, float cz, float upx, float upy, float upz);
};

#endif