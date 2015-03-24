#ifndef CSCIx239
#define CSCIx239

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#ifdef USEGLEW
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))

#ifdef __cplusplus
extern "C" {
#endif

void         Print(const char* format , ...);
void         Fatal(const char* format , ...);
void         ErrCheck(const char* where);
unsigned int LoadTexBMP(const char* file);
void         Project(double fov,double asp,double dim);
int          LoadOBJ(const char* file);
int          FramesPerSecond(void);
double       Elapsed(void);
void         CreateShader(int prog,const GLenum type,const char* file);
void         PrintProgramLog(int obj);
int          CreateShaderProg(const char* VertFile,const char* FragFile);
int          CreateNoise3D(int unit);

// cube.c
void         InitCube();
void         DrawCube(int shader, int heightMap, int texture, int normalMap);

// light.c
void         InitLight(int shaders[], int numShaders);
void         SetLightPosition(float position[]);

// projection.c
void         InitProjection(int shaders[], int numShaders);
void         SetProjectionMatrix(float fov, float asp, float zNear, float zFar);
void         SetModelViewMatrix(float ex, float ey, float ez, float cx, float cy, float cz, float upx, float upy, float upz);

#ifdef __cplusplus
}
#endif

#endif

