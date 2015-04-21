#include <Renderer.h>
#include <Light.h>
#include <Material.h>
#include <iostream>
#include <GameWindow.h>

unsigned int shadowtex;

Renderer::Renderer() {
    scene = NULL;
    camera = NULL;
    th=0;       //  Azimuth of view angle
    ph=0;       //  Elevation of view angle
    zh=0;       //  Light angle
    fov=55.0;   //  Field of view (angles)
    asp=1;      //  Screen aspect ratio
    defaultBasicShader = -1;
    defaultLightShader = -1;
}

Renderer::~Renderer() {
    // Delete scene
    if (scene)
        delete scene;
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

    // Bind to light and material as well
    Light::bindLightBuffers(shader);
    Material::bindMaterialBuffers(shader);

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

Scene * Renderer::getScene() {
    return this->scene;
}


void Renderer::setCamera(Camera * c) {
    this->camera = c;
}

Camera * Renderer::getCamera() {
    return this->camera;
}

void Renderer::display() {
    // Update scripts
    if (scene)
        scene->updateScripts();

    // Enable z-buffer 
    glEnable(GL_DEPTH_TEST);
    // Disable culling
    glDisable(GL_CULL_FACE);
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
        scene->drawObjectsWithLights();

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
    //  Ratio of the width to the height of the window
    asp = (height>0) ? (double)width/height : 1;
    //  Set the viewport to the entire window
    glViewport(0,0, width,height);
    if (camera) {
        //  Set model view matrix
        float Ex = -2*GameWindow::dim*CsSin(th)*CsCos(ph);
        float Ey = +2*GameWindow::dim        *CsSin(ph);
        float Ez = +2*GameWindow::dim*CsCos(th)*CsCos(ph);
        camera->lookAt(Ex, Ey, Ez, 0, 0, 0, 0, CsCos(ph), 0);
        //  Set projection
        camera->setPerspective(M_PI*fov/180.0, asp, GameWindow::dim/16.0, GameWindow::dim*16.0);
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
        GameWindow::dim += 0.1;
    //  PageDown key - decrease dim
    else if (key == GLUT_KEY_PAGE_UP && GameWindow::dim>1)
        GameWindow::dim -= 0.1;
    //  Keep angles to +/-360 degrees
    th %= 360;
    ph %= 360;
    if (camera) {
        //  Set model view matrix
        float Ex = -2*GameWindow::dim*CsSin(th)*CsCos(ph);
        float Ey = +2*GameWindow::dim        *CsSin(ph);
        float Ez = +2*GameWindow::dim*CsCos(th)*CsCos(ph);
        camera->lookAt(Ex, Ey, Ez, 0, 0, 0, 0, CsCos(ph), 0);
        //  Set projection
        camera->setPerspective(M_PI*fov/180.0, asp, GameWindow::dim/16.0, GameWindow::dim*16.0);
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
