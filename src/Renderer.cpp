#include <Renderer.h>
#include <Light.h>
#include <Material.h>
#include <iostream>
#include <GameWindow.h>
#include <Input.h>

unsigned int shadowtex;

Renderer::Renderer() {
    scene = NULL;
    camera = NULL;
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
	// Update up
	Input::updateDownMap();
    // Update scripts
    if (scene)
        scene->updateScripts();
    // Reset Input
    Input::resetMaps();

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
    	camera->setAspectRatio(asp);
    }
}
void Renderer::special(int key, int x, int y) {
    //  Right arrow key - increase angle by 5 degrees
    if (key == GLUT_KEY_RIGHT) {
    	Input::setKey(KeyCode::RightArrow);
    }
    //  Left arrow key - decrease angle by 5 degrees
    else if (key == GLUT_KEY_LEFT) {
    	Input::setKey(KeyCode::LeftArrow);
    }
	//  Up arrow key - increase elevation by 5 degrees
	else if (key == GLUT_KEY_UP) {
		Input::setKey(KeyCode::UpArrow);
	}
	//  Down arrow key - decrease elevation by 5 degrees
	else if (key == GLUT_KEY_DOWN) {
		Input::setKey(KeyCode::DownArrow);
	}
    //  PageUp key - increase dim
    else if (key == GLUT_KEY_PAGE_DOWN) {
    	Input::setKey(KeyCode::PageDown);
    }
    //  PageDown key - decrease dim
    else if (key == GLUT_KEY_PAGE_UP) {
    	Input::setKey(KeyCode::PageUp);
    }
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
}
void Renderer::key(unsigned char ch, int x, int y) {
	// Set input
	Input::setKey(ch);
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
}
void Renderer::idle() {
    // Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}
