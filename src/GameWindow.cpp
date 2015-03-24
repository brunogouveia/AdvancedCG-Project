/*
 * GameWindow.cpp
 *
 *  Created on: Mar 23, 2015
 *      Author: Bruno Gouveia
 */

#include "GameWindow.h"

Renderer * GameWindow::renderer = NULL;
string GameWindow::windowName;
int GameWindow::width;
int GameWindow::height;

GameWindow::GameWindow(string wn, int w, int h) {
    // Save data
    windowName = wn;
    width = w;
    height = h;
}

GameWindow::~GameWindow() {
    // do nothing
}

bool GameWindow::init(Renderer * r, int * argc, char ** argv) {
    // Initialize GLUT
    glutInit(argc, argv);
    // Request double buffer, true color and z buffering
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    // Set window size
    glutInitWindowSize(width, height);
    // Create window
    glutCreateWindow(windowName.c_str());
#ifdef USEGLEW
   //  Initialize GLEW
   if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
   if (!GLEW_VERSION_4_0) Fatal("OpenGL 4.0 not supported\n");
#endif
   printf("OpenGL version supported by this platform (%s): \n", glGetString(GL_VERSION));
    // Set callback functions
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(special);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    // Set renderer
    renderer = r;

    return true;
}

void GameWindow::mainLoop() {
    //  Pass control to GLUT so it can interact with the user
    glutMainLoop();
}

void GameWindow::display() {
    renderer->display();
}
void GameWindow::reshape(int width, int height) {
    renderer->reshape(width, height);
}
void GameWindow::special(int key, int x, int y) {
    renderer->special(key, x, y);
}
void GameWindow::key(unsigned char ch, int x, int y) {
    renderer->key(ch, x, y);
}
void GameWindow::idle() {
    renderer->idle();
}
