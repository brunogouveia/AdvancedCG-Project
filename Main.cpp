#include <GameWindow.h>
#include <Cube.h>

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

	Cube cube;
	cube.init(r.createShaderProg("shaders/phong.vert","shaders/phong.frag"));
	s.addGameObject(&cube);
	// Call main loop
	GameWindow::mainLoop();
	return 0;
}
