# Example 22
EXE=GE

INCLUDE_DIR=include/
BUILD_DIR=build/
SRC_DIR=src/

# Main target
all: $(EXE)

#  MinGW
ifeq "$(OS)" "Windows_NT"
CFLG=-O3 -Wall -DUSEGLEW
LIBS=-lglew32 -lglut32cu -lglu32 -lopengl32
CLEAN=del *.exe build\*.o *.a
else
#  OSX
ifeq "$(shell uname)" "Darwin"
CFLG=-O3 -Wall -Wno-deprecated-declarations
LIBS=-framework GLUT -framework OpenGL
#  Linux/Unix/Solaris
else
CFLG=-O3 -Wall
LIBS=-lglut -lGLU -lGL -lm
endif
#  OSX/Linux/Unix/Solaris
CLEAN=rm -f $(EXE) $(BUILD_DIR)*.o *.a
endif



# Dependencies
OBJS = \
	$(BUILD_DIR)GameWindow.o \
	$(BUILD_DIR)Input.o \
	$(BUILD_DIR)Renderer.o \
	$(BUILD_DIR)Scene.o \
	$(BUILD_DIR)Camera.o \
	$(BUILD_DIR)GBuffer.o \
	$(BUILD_DIR)Transform.o \
	$(BUILD_DIR)GameObject.o \
	$(BUILD_DIR)MeshObject.o \
	$(BUILD_DIR)tiny_obj_loader.o \
	$(BUILD_DIR)Script.o \
	$(BUILD_DIR)Light.o \
	$(BUILD_DIR)Material.o \
	$(BUILD_DIR)Texture.o \
	$(BUILD_DIR)Cube.o \
	$(BUILD_DIR)XmlParser.o \
	$(BUILD_DIR)fatal.o \
	$(BUILD_DIR)loadtexbmp.o \
	$(BUILD_DIR)print.o \
	$(BUILD_DIR)project.o \
	$(BUILD_DIR)errcheck.o \
	$(BUILD_DIR)object.o \
	$(BUILD_DIR)fps.o \
	$(BUILD_DIR)elapsed.o \
	$(BUILD_DIR)shader.o \
	$(BUILD_DIR)noise.o

#  Create archive
CSCIx239.a: $(OBJS)
	ar -rcs CSCIx239.a $(OBJS)

# Compile rules
.c.o: 
	g++ -c $(CFLG) -I$(INCLUDE_DIR) $<
.cpp.o:
	g++ -c $(CFLG) -I$(INCLUDE_DIR) $<

#  Link
GE: $(BUILD_DIR)Main.o CSCIx239.a
	g++ -O3 -o GE -I$(INCLUDE_DIR) -I$(SRC_DIR) $(BUILD_DIR)Main.o CSCIx239.a $(LIBS) $(SRC_DIR)scripts/*.cpp $(SRC_DIR)Util/*.cpp 


#  Clean
clean:
	$(CLEAN)

# Generate .o
$(BUILD_DIR)Main.o: Main.cpp
	g++ -c -o $(BUILD_DIR)Main.o $(CFLG) -I$(INCLUDE_DIR) Main.cpp

$(BUILD_DIR)GameWindow.o: $(SRC_DIR)GameWindow.cpp
	g++ -c -o $(BUILD_DIR)GameWindow.o $(CFLG) -I$(INCLUDE_DIR) $(SRC_DIR)GameWindow.cpp

$(BUILD_DIR)Input.o: $(SRC_DIR)Input.cpp
	g++ -c -o $(BUILD_DIR)Input.o $(CFLG) -I$(INCLUDE_DIR) $(SRC_DIR)Input.cpp

$(BUILD_DIR)Renderer.o: $(SRC_DIR)Renderer.cpp
	g++ -c -o $(BUILD_DIR)Renderer.o $(CFLG) -I$(INCLUDE_DIR) $(SRC_DIR)Renderer.cpp

$(BUILD_DIR)Camera.o: $(SRC_DIR)Camera.cpp
	g++ -c -o $(BUILD_DIR)Camera.o $(CFLG) -I$(INCLUDE_DIR) $(SRC_DIR)Camera.cpp

$(BUILD_DIR)GBuffer.o: $(SRC_DIR)GBuffer.cpp
	g++ -c -o $(BUILD_DIR)GBuffer.o $(CFLG) -I$(INCLUDE_DIR) $(SRC_DIR)GBuffer.cpp

$(BUILD_DIR)Scene.o: $(SRC_DIR)Scene.cpp
	g++ -c -o $(BUILD_DIR)Scene.o $(CFLG) -I$(INCLUDE_DIR) $(SRC_DIR)Scene.cpp

$(BUILD_DIR)Transform.o: $(SRC_DIR)Transform.cpp
	g++ -c -o $(BUILD_DIR)Transform.o $(CFLG) -I$(INCLUDE_DIR) $(SRC_DIR)Transform.cpp

$(BUILD_DIR)GameObject.o: $(SRC_DIR)gameobject/GameObject.cpp
	g++ -c -o $(BUILD_DIR)GameObject.o $(CFLG) -I$(INCLUDE_DIR) $(SRC_DIR)gameobject/GameObject.cpp

$(BUILD_DIR)MeshObject.o: $(SRC_DIR)gameobject/MeshObject.cpp
	g++ -c -o $(BUILD_DIR)MeshObject.o $(CFLG) -I$(INCLUDE_DIR) $(SRC_DIR)gameobject/MeshObject.cpp

$(BUILD_DIR)tiny_obj_loader.o: $(SRC_DIR)obj/tiny_obj_loader.cpp
	g++ -c -o $(BUILD_DIR)tiny_obj_loader.o $(CFLG) -I$(INCLUDE_DIR) $(SRC_DIR)obj/tiny_obj_loader.cpp

$(BUILD_DIR)Script.o: $(SRC_DIR)Script.cpp
	g++ -c -o $(BUILD_DIR)Script.o $(CFLG) -I$(INCLUDE_DIR) $(SRC_DIR)Script.cpp

$(BUILD_DIR)Light.o: $(SRC_DIR)Light.cpp
	g++ -c -o $(BUILD_DIR)Light.o $(CFLG) -I$(INCLUDE_DIR) $(SRC_DIR)Light.cpp

$(BUILD_DIR)Material.o: $(SRC_DIR)Material.cpp
	g++ -c -o $(BUILD_DIR)Material.o $(CFLG) -I$(INCLUDE_DIR) $(SRC_DIR)Material.cpp

$(BUILD_DIR)Texture.o: $(SRC_DIR)Texture.cpp
	g++ -c -o $(BUILD_DIR)Texture.o $(CFLG) -I$(INCLUDE_DIR) $(SRC_DIR)Texture.cpp

$(BUILD_DIR)Cube.o: $(SRC_DIR)gameobject/Cube.cpp
	g++ -c -o $(BUILD_DIR)Cube.o $(CFLG) -I$(INCLUDE_DIR) $(SRC_DIR)gameobject/Cube.cpp

$(BUILD_DIR)XmlParser.o: $(SRC_DIR)xml/XmlParser.cpp
	g++ -c -o $(BUILD_DIR)XmlParser.o $(CFLG) -I$(INCLUDE_DIR) $(SRC_DIR)xml/XmlParser.cpp

$(BUILD_DIR)fatal.o: $(SRC_DIR)fatal.c
	g++ -c -o $(BUILD_DIR)fatal.o $(CFLG) -I$(INCLUDE_DIR) $(SRC_DIR)fatal.c

$(BUILD_DIR)loadtexbmp.o: $(SRC_DIR)loadtexbmp.c
	g++ -c -o $(BUILD_DIR)loadtexbmp.o $(CFLG) -I$(INCLUDE_DIR) $(SRC_DIR)loadtexbmp.c

$(BUILD_DIR)print.o: $(SRC_DIR)print.c
	g++ -c -o $(BUILD_DIR)print.o $(CFLG) -I$(INCLUDE_DIR) $(SRC_DIR)print.c

$(BUILD_DIR)project.o: $(SRC_DIR)project.c
	g++ -c -o $(BUILD_DIR)project.o $(CFLG) -I$(INCLUDE_DIR) $(SRC_DIR)project.c

$(BUILD_DIR)errcheck.o: $(SRC_DIR)errcheck.c
	g++ -c -o $(BUILD_DIR)errcheck.o $(CFLG) -I$(INCLUDE_DIR) $(SRC_DIR)errcheck.c

$(BUILD_DIR)object.o: $(SRC_DIR)object.c
	g++ -c -o $(BUILD_DIR)object.o $(CFLG) -I$(INCLUDE_DIR) $(SRC_DIR)object.c

$(BUILD_DIR)fps.o: $(SRC_DIR)fps.c
	g++ -c -o $(BUILD_DIR)fps.o $(CFLG) -I$(INCLUDE_DIR) $(SRC_DIR)fps.c

$(BUILD_DIR)elapsed.o: $(SRC_DIR)elapsed.c
	g++ -c -o $(BUILD_DIR)elapsed.o $(CFLG) -I$(INCLUDE_DIR) $(SRC_DIR)elapsed.c

$(BUILD_DIR)shader.o: $(SRC_DIR)shader.c
	g++ -c -o $(BUILD_DIR)shader.o $(CFLG) -I$(INCLUDE_DIR) $(SRC_DIR)shader.c

$(BUILD_DIR)noise.o: $(SRC_DIR)noise.c
	g++ -c -o $(BUILD_DIR)noise.o $(CFLG) -I$(INCLUDE_DIR) $(SRC_DIR)noise.c