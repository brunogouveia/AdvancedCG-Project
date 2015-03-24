#ifndef CSCIx239
#include "CSCIx239.h"
#endif

// Buffer
static int lightBindingPoint = 2;
static unsigned int lightBuffer;
// Light initial data
static const float lightData[] =
{
   //Position
   2, 0, 0, 1,
   //Global
   0.1, 0.1, 0.1, 1,
   //Ambient
   0.1, 0.1, 0.1, 1.0,
   // Diffuse
   1.0, 1.0, 1.0, 1.0,
   // Specular
   1.0, 1.0, 1.0, 1.0
};

void InitLight(int shaders[], int numShaders) {
   int i;
   // Set binding point for each shader
   for (i = 0; i < numShaders; ++i) {
      // Get uniform block index
      int idBlock = glGetUniformBlockIndex(shaders[i], "Light");
      // Set bindint point
      if (idBlock >= 0) glUniformBlockBinding(shaders[i], idBlock, lightBindingPoint);
   }

   // Generate uniform buffer
   glGenBuffers(1, &lightBuffer);
   glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);

   // Copy data to buffer
   glBufferData(GL_UNIFORM_BUFFER, sizeof(lightData), lightData, GL_DYNAMIC_DRAW);
   glBindBufferBase(GL_UNIFORM_BUFFER, lightBindingPoint, lightBuffer);

   // Unbind buffer
   glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void SetLightPosition(float position[]) {
   // Bind buffer
   glBindBuffer(GL_UNIFORM_BUFFER, lightBuffer);
   // Set position
   glBufferSubData(GL_UNIFORM_BUFFER, 0, 4*sizeof(float), position);
   // Unbind buffer
   glBindBuffer(GL_UNIFORM_BUFFER, 0);
}