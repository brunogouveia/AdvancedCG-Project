#ifndef CSCIx239
#include "CSCIx239.h"
#endif
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"

// Private data
static const int projectionBindingPoint = 1;
static unsigned int projectionBuffer;
// Matrices
static glm::mat4 projectionMatrix;
static glm::mat4 modelViewMatrix;
static glm::mat4 normalMatrix;
static glm::mat4 MVPMatrix;

void InitProjection(int shaders[], int numShaders) {
	int i;
	for (i = 0; i < numShaders; ++i) {
		// Find uniform block id
		int idBlock = glGetUniformBlockIndex(shaders[i], "Projection");
		// Set binding point
		if (idBlock >= 0) glUniformBlockBinding(shaders[i], idBlock, projectionBindingPoint);
	}

	// Create buffer
	glGenBuffers(1, &projectionBuffer);
	glBindBuffer(GL_UNIFORM_BUFFER, projectionBuffer);

	// Initalize data
	float data[16*4] = {0}; // 4 mat4
	glBufferData(GL_UNIFORM_BUFFER, sizeof(data), data, GL_DYNAMIC_DRAW);

	// Bind to binding point
	glBindBufferBase(GL_UNIFORM_BUFFER, projectionBindingPoint, projectionBuffer);

	// Unbind buffer
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SetProjectionMatrix(float fov, float asp, float zNear, float zFar) {
	// Create new projection matrix
	projectionMatrix = glm::perspective(fov, asp, zNear, zFar);

	// Bind buffer
	glBindBuffer(GL_UNIFORM_BUFFER, projectionBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, 16*sizeof(float), glm::value_ptr(projectionMatrix));

	// Unbind buffer
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SetModelViewMatrix(float ex, float ey, float ez, float cx, float cy, float cz, float upx, float upy, float upz) {
	// Create new model view matrix
	modelViewMatrix = glm::lookAt(glm::vec3(ex,ey,ez), glm::vec3(cx,cy,cz), glm::vec3(upx, upy, upz));
	// Set normal matrix too
	normalMatrix = glm::inverseTranspose(modelViewMatrix);

	// Bind buffer and copy data
	glBindBuffer(GL_UNIFORM_BUFFER, projectionBuffer);
	glBufferSubData(GL_UNIFORM_BUFFER, 16*sizeof(float), 16*sizeof(float), glm::value_ptr(modelViewMatrix));
	glBufferSubData(GL_UNIFORM_BUFFER, 32*sizeof(float), 16*sizeof(float), glm::value_ptr(normalMatrix));

	// Unbind buffer
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}