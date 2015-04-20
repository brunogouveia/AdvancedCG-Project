#include <GameWindow.h>
#include <Cube.h>
#include <Light.h>
#include <Script.h>
#include <xml/XmlParser.h>

int main(int argc, char ** argv) {

	// int h=800, w=800;
	XmlParser::loadFromXml("scene.xml", &argc, argv);

	// // Create a game window
	// GameWindow::setWindow("GE", h, w);

	// // Initailize it
	// GameWindow::init(&argc, argv);
	// // Create scene
	// Scene s;
	// Renderer r(&s);
	// GameWindow::setRenderer(&r);
	// r.setScene(&s);
	// r.setCamera(new Camera());

	// Light::init();	

	// glm::mat4 id;

	// Cube cube(id);
	// cube.init(r.createShaderProg("shaders/phong.vert","shaders/phong.frag"), r.createShaderProg("shaders/phongl.vert","shaders/phongl.frag"));
	// Texture cubeTexture(GL_TEXTURE0, "textures/pi.bmp");
	// cube.setTexture(cubeTexture);
	// s.addGameObject(&cube);

	// Light firstLight;
	// firstLight.setPosition(1.438680, 2.0, 1.3893);
	// s.addLight(&firstLight);

	// // s.addScript(Script::GetScriptByName(&cube, "CubeScript"));
	// s.addScript(Script::GetScriptByName(&cube, "LightScript"));

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
