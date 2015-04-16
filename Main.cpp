#include <GameWindow.h>
#include <Cube.h>
#include <Light.h>
#include <Script.h>

int main(int argc, char ** argv) {


	// Create a game window
	GameWindow gw("GE", 800, 800);

	// Initailize it
	GameWindow::init(&argc, argv);
	// Create scene
	Scene s;
	Renderer r(&s);
	GameWindow::setRenderer(&r);
	r.setScene(&s);
	r.setCamera(new Camera());

	Light::init();	

	glm::mat4 id;

	Cube cube(id);
	cube.init(r.createShaderProg("shaders/phong.vert","shaders/phong.frag"), r.createShaderProg("shaders/phongl.vert","shaders/phongl.frag"));
	Script script(&cube);
	Texture cubeTexture(GL_TEXTURE0, "textures/pi.bmp");
	cube.setTexture(cubeTexture);
	s.addGameObject(&cube);
	s.addScript(&script);

	Light firstLight;
	firstLight.setPosition(1.438680, 2.0, 1.3893);
	s.addLight(&firstLight);

	// Cube cube2(glm::translate(id, glm::vec3(-1.0)));
	// cube2.init(r.createShaderProg("shaders/phong.vert","shaders/phong.frag"));
	// Texture cube2Texture(GL_TEXTURE0, "textures/texture.bmp");
	// cube2.setTexture(cube2Texture);
	// s.addGameObject(&cube2);

	//  Make sure enough texture units are available
    int n;
    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,&n);
    printf("%d\n", n);
	// Call main loop
	GameWindow::mainLoop();
	return 0;
}
