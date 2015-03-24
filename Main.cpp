#include <GameWindow.h>
#include <Cube.h>

int main(int argc, char ** argv) {
	// Create scene
	Scene s;

	Renderer r(&s);

	// Create a game window
	GameWindow gw("GE", 800, 600);

	// Initailize it
	GameWindow::init(&r, &argc, argv);

	Cube cube;
	cube.init(CreateShaderProg("shaders/phong.vert","shaders/phong.frag"));
	// Call main loop
	GameWindow::mainLoop();
	return 0;
}
