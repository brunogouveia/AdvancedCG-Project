/*
 * GameWindow.h
 *
 *  Created on: Mar 23, 2015
 *      Author: Bruno Gouveia
 */

#ifndef GAMEWINDOW_H_
#define GAMEWINDOW_H_

#include <string>
#include <CSCIx239.h>
#include <Renderer.h>

using namespace std;

class GameWindow {
private:
    // Renderer
    static Renderer * renderer;

    static string windowName;
    static int width;
    static int height;

public:
    GameWindow(string windowName, int width, int height);
    virtual ~GameWindow();

    static void setRenderer(Renderer * r);
    static Renderer * getRenderer();
    static bool init(int * argc, char ** argv);
    static void mainLoop();

    // Callback functions
    static void display();
    static void reshape(int width, int height);
    static void special(int key,int x,int y);
    static void key(unsigned char ch,int x,int y);
    static void idle();
};

#endif /* GAMEWINDOW_H_ */