#include <Renderer.h>
#include <Light.h>
#include <iostream>

unsigned int shadowtex;

Renderer::Renderer(Scene * s) {
    scene = s;
    camera = NULL;
    th=0;       //  Azimuth of view angle
    ph=0;       //  Elevation of view angle
    zh=0;       //  Light angle
    fov=55.0;   //  Field of view (angles)
    asp=1;      //  Screen aspect ratio
    dim=3.0;      //  World dimension
    defaultBasicShader = -1;
    defaultLightShader = -1;

    // initShadowMap();
    // shadowShader = createShaderProg("shaders/shadow.vert", "shaders/shadow.frag");
}

Renderer::~Renderer() {
    
}

void Renderer::initShadowMap() {
    // unsigned int shadowtex; //  Shadow buffer texture id
    int shadowdim = 2048;
    int n;
    
    shadowShader = createShaderProg("shaders/shadow.vert", "shaders/shadow.frag");
    //  Make sure multi-textures are supported
    glGetIntegerv(GL_MAX_TEXTURE_UNITS,&n);
    if (n<2) Fatal("Multiple textures not supported\n");
    
    //  Get maximum texture buffer size
    glGetIntegerv(GL_MAX_TEXTURE_SIZE,&shadowdim);
    //  Limit texture size to maximum buffer size
    glGetIntegerv(GL_MAX_RENDERBUFFER_SIZE,&n);
    if (shadowdim>n) shadowdim = n;
    //  Limit texture size to 2048 for performance
    if (shadowdim>2048) shadowdim = 2048;
    if (shadowdim<512) Fatal("Shadow map dimensions too small %d\n",shadowdim);
    
    //  Do Shadow textures in MultiTexture 1
    glActiveTexture(GL_TEXTURE1);
    
    //  Allocate and bind shadow texture
    glGenTextures(1,&shadowtex);
    glBindTexture(GL_TEXTURE_2D,shadowtex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, shadowdim, shadowdim, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    
    //  Map single depth value to RGBA (this is called intensity)
    glTexParameteri(GL_TEXTURE_2D,GL_DEPTH_TEXTURE_MODE,GL_INTENSITY);
    
    //  Set texture mapping to clamp and linear interpolation
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    
    // Switch back to default textures
    glActiveTexture(GL_TEXTURE0);
    
    // Attach shadow texture to frame buffer
    glGenFramebuffers(1,&depthFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER,depthFrameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowtex, 0);
    //  Don't write or read to visible color buffer
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    //  Make sure this all worked
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) Fatal("Error setting up frame buffer\n");
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

int Renderer::createShaderProg(std::string vertShaderFile, std::string fragShaderFile) {
    // Create shader
    int shader = CreateShaderProg(vertShaderFile.c_str(), fragShaderFile.c_str());

    // We need to bind this shader with the projection buffer
    if (camera) {
        camera->bindProjectionBuffer(shader);
    } else {
        std::cout << "Renderer::createShaderProg - camera is NULL" << std::endl;
    }

    // Bind light as well
    Light::bindLightBuffers(shader);

    // Return shader
    return shader;
}

int Renderer::getDefaultBasicShader() {
    if (defaultBasicShader == -1)
        defaultBasicShader = createShaderProg("shaders/phong.vert","shaders/phong.frag");
    return defaultBasicShader;
}

int Renderer::getDefaultLightShader() {
    if (defaultLightShader == -1)
        defaultLightShader = createShaderProg("shaders/phongl.vert","shaders/phongl.frag");
    return defaultLightShader;
}

void Renderer::setScene(Scene * s) {
    this->scene = s;
}

void Renderer::setCamera(Camera * c) {
    this->camera = c;
    initShadowMap();
}

void Renderer::display() {
    // Update scripts
    scene->updateScripts();

    // Enable z-buffer and culling
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    // Set depth test function
    glDepthFunc(GL_LEQUAL);

    // Clear color and depth buffer
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw scene with basic shader
    if (scene)
        scene->firstDraw();

    // Enable additive blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    // Set depth test function
    glDepthFunc(GL_EQUAL);

    // Draw scene with LIGHT shader
    if (scene)
        scene->drawObjectsWithLights(depthFrameBuffer, shadowShader, shadowtex);

    // Disable blending
    glDisable(GL_BLEND);

    //  Display parameters
    glColor3f(1,1,1);
    glWindowPos2i(5,5);
    Print("FPS=%d Angle=%d,%d",
      FramesPerSecond(),th,ph);
    //  Render the scene and make it visible
    ErrCheck("display");
    glFlush();
    glutSwapBuffers();
}
void Renderer::reshape(int width, int height) {
    // Update local data
    this->width = width;
    this->height = height;
    //  Ratio of the width to the height of the window
    asp = (height>0) ? (double)width/height : 1;
    //  Set the viewport to the entire window
    glViewport(0,0, width,height);
    if (camera) {
        //  Set model view matrix
        float Ex = -2*dim*CsSin(th)*CsCos(ph);
        float Ey = +2*dim        *CsSin(ph);
        float Ez = +2*dim*CsCos(th)*CsCos(ph);
        camera->lookAt(Ex, Ey, Ez, 0, 0, 0, 0, CsCos(ph), 0);
        //  Set projection
        camera->setPerspective(M_PI*fov/180.0, asp, dim/16.0, dim*16.0);
    }
}
void Renderer::special(int key, int x, int y) {
    //  Right arrow key - increase angle by 5 degrees
    if (key == GLUT_KEY_RIGHT)
      th += 5;
    //  Left arrow key - decrease angle by 5 degrees
    else if (key == GLUT_KEY_LEFT)
      th -= 5;
    //  Up arrow key - increase elevation by 5 degrees
    else if (key == GLUT_KEY_UP)
      ph += 5;
    //  Down arrow key - decrease elevation by 5 degrees
    else if (key == GLUT_KEY_DOWN)
      ph -= 5;
    //  PageUp key - increase dim
    else if (key == GLUT_KEY_PAGE_DOWN)
        dim += 0.1;
    //  PageDown key - decrease dim
    else if (key == GLUT_KEY_PAGE_UP && dim>1)
        dim -= 0.1;
    //  Keep angles to +/-360 degrees
    th %= 360;
    ph %= 360;
    if (camera) {
        //  Set model view matrix
        float Ex = -2*dim*CsSin(th)*CsCos(ph);
        float Ey = +2*dim        *CsSin(ph);
        float Ez = +2*dim*CsCos(th)*CsCos(ph);
        camera->lookAt(Ex, Ey, Ez, 0, 0, 0, 0, CsCos(ph), 0);
        //  Set projection
        camera->setPerspective(M_PI*fov/180.0, asp, dim/16.0, dim*16.0);
    }
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
}
void Renderer::key(unsigned char ch, int x, int y) {
    //  Exit on ESC
    if (ch == 27)
        exit(0);
    //  Reset view angle
    else if (ch == '0')
        th = ph = 0;
    //  Move light
    else if (ch == '[')
        zh += 5;
    else if (ch == ']')
        zh -= 5;
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
}
void Renderer::idle() {
    // Update scripts
    scene->updateScripts();
    // Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}
