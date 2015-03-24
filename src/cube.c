#ifndef CSCIx239
#include "CSCIx239.h"
#endif

// Private data
static unsigned int cubeVertexArray;
static unsigned int cubeBuffer;
static const unsigned int numVertices = 36;
static float cubeData[] = {
//  X  Y  Z  W    Nx Ny Nz   Tx Ty Tz    R G B   s t
   //  Front
   +1,+1,+1,+1,   0, 0,+1,   +1, 0, 0,   1,0,0,  1,1,
   -1,+1,+1,+1,   0, 0,+1,   +1, 0, 0,   1,0,0,  0,1,
   +1,-1,+1,+1,   0, 0,+1,   +1, 0, 0,   1,0,0,  1,0,
   -1,+1,+1,+1,   0, 0,+1,   +1, 0, 0,   1,0,0,  0,1,
   +1,-1,+1,+1,   0, 0,+1,   +1, 0, 0,   1,0,0,  1,0,
   -1,-1,+1,+1,   0, 0,+1,   +1, 0, 0,   1,0,0,  0,0,
   //  Back
   -1,-1,-1,+1,   0, 0,-1,   -1, 0, 0,   0,0,1,  1,0,
   +1,-1,-1,+1,   0, 0,-1,   -1, 0, 0,   0,0,1,  0,0,
   -1,+1,-1,+1,   0, 0,-1,   -1, 0, 0,   0,0,1,  1,1,
   +1,-1,-1,+1,   0, 0,-1,   -1, 0, 0,   0,0,1,  0,0,
   -1,+1,-1,+1,   0, 0,-1,   -1, 0, 0,   0,0,1,  1,1,
   +1,+1,-1,+1,   0, 0,-1,   -1, 0, 0,   0,0,1,  0,1,
   //  Right
   +1,+1,+1,+1,  +1, 0, 0,    0, 0,-1,   1,1,0,  0,1,
   +1,-1,+1,+1,  +1, 0, 0,    0, 0,-1,   1,1,0,  0,0,
   +1,+1,-1,+1,  +1, 0, 0,    0, 0,-1,   1,1,0,  1,1,
   +1,-1,+1,+1,  +1, 0, 0,    0, 0,-1,   1,1,0,  0,0,
   +1,+1,-1,+1,  +1, 0, 0,    0, 0,-1,   1,1,0,  1,1,
   +1,-1,-1,+1,  +1, 0, 0,    0, 0,-1,   1,1,0,  1,0,
   //  Left
   -1,+1,+1,+1,  -1, 0, 0,    0, 0,+1,   0,1,0,  1,1,
   -1,+1,-1,+1,  -1, 0, 0,    0, 0,+1,   0,1,0,  0,1,
   -1,-1,+1,+1,  -1, 0, 0,    0, 0,+1,   0,1,0,  1,0,
   -1,+1,-1,+1,  -1, 0, 0,    0, 0,+1,   0,1,0,  0,1,
   -1,-1,+1,+1,  -1, 0, 0,    0, 0,+1,   0,1,0,  1,0,
   -1,-1,-1,+1,  -1, 0, 0,    0, 0,+1,   0,1,0,  0,0,
   //  Top
   +1,+1,+1,+1,   0,+1, 0,   +1, 0, 0,   0,1,1,  1,0,
   +1,+1,-1,+1,   0,+1, 0,   +1, 0, 0,   0,1,1,  1,1,
   -1,+1,+1,+1,   0,+1, 0,   +1, 0, 0,   0,1,1,  0,0,
   +1,+1,-1,+1,   0,+1, 0,   +1, 0, 0,   0,1,1,  1,1,
   -1,+1,+1,+1,   0,+1, 0,   +1, 0, 0,   0,1,1,  0,0,
   -1,+1,-1,+1,   0,+1, 0,   +1, 0, 0,   0,1,1,  0,1,
   //  Bottom
   -1,-1,-1,+1,   0,-1, 0,   +1, 0, 0,   1,0,1,  0,0,
   +1,-1,-1,+1,   0,-1, 0,   +1, 0, 0,   1,0,1,  1,0,
   -1,-1,+1,+1,   0,-1, 0,   +1, 0, 0,   1,0,1,  0,1,
   +1,-1,-1,+1,   0,-1, 0,   +1, 0, 0,   1,0,1,  1,0,
   -1,-1,+1,+1,   0,-1, 0,   +1, 0, 0,   1,0,1,  0,1,
   +1,-1,+1,+1,   0,-1, 0,   +1, 0, 0,   1,0,1,  1,1,
};

void InitCube() {
	// Create VAO
	glGenVertexArrays(1,&cubeVertexArray);
	glBindVertexArray(cubeVertexArray);
	// Create cube buffer
	glGenBuffers(1,&cubeBuffer);
	glBindBuffer(GL_ARRAY_BUFFER,cubeBuffer);
	glBufferData(GL_ARRAY_BUFFER,sizeof(cubeData),cubeData,GL_STATIC_DRAW);

	// Unbind this array and buffer
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
}

void DrawCube(int shader, int heightMap, int texture, int normalMap) {
	// Bind shader
	glUseProgram(shader);

	// Bind array and buffer
	glBindVertexArray(cubeVertexArray);
	glBindBuffer(GL_ARRAY_BUFFER,cubeBuffer);

	// Set attribute 0 - Vertex (vec4)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,15*sizeof(float),(void*)0);
	// Set attribute 1 - Normal (vec3)
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,15*sizeof(float),(void*)(4*sizeof(float)));
   // Set attribute 2 - Tangent (vec3)
   glEnableVertexAttribArray(2);
   glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,15*sizeof(float),(void*)(7*sizeof(float)));
   // Set attribute 3 - Color (vec3)
   glEnableVertexAttribArray(3);
   glVertexAttribPointer(3,3,GL_FLOAT,GL_FALSE,15*sizeof(float),(void*)(10*sizeof(float)));
   // Set attribute 4 - Texture coordinates (vec2)
   glEnableVertexAttribArray(4);
   glVertexAttribPointer(4,2,GL_FLOAT,GL_FALSE,15*sizeof(float),(void*)(13*sizeof(float)));

   // Set textures
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, texture);
   int id = glGetUniformLocation(shader, "text");
   if (id >= 0) glUniform1i(id, 0);
   glActiveTexture(GL_TEXTURE1);
   glBindTexture(GL_TEXTURE_2D, heightMap);
   id = glGetUniformLocation(shader, "heightMap");
   if (id >= 0) glUniform1i(id, 1);
   glActiveTexture(GL_TEXTURE2);
   glBindTexture(GL_TEXTURE_2D, normalMap);
   id = glGetUniformLocation(shader, "normalMap");
   if (id >= 0) glUniform1i(id, 2);

	// Draw cube
	glDrawArrays(GL_PATCHES,0,numVertices);

	// Disable attributes
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);

	// Unbind everything
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glUseProgram(0);
}