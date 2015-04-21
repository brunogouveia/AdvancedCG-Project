#include <GameWindow.h>
#include <Cube.h>
#include <Light.h>
#include <Script.h>
#include <xml/XmlParser.h>

int main(int argc, char ** argv) {

	// Read;
	XmlParser::loadFromXml("scene.xml", &argc, argv);

	//  Make sure enough texture units are available
    int n;
    glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,&n);
    printf("%d\n", n);
	// Call main loop
	GameWindow::mainLoop();
	return 0;
}
