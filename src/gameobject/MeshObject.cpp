#include <MeshObject.h>
#include <obj/glm.h>
#include <GameWindow.h>

MeshObject::MeshObject(glm::mat4 modelMatrix) {
	this->modelMatrix = modelMatrix;
	data = NULL;
}

MeshObject::~MeshObject() {
	if (data)
		delete data;
}

void MeshObject::init(int basicShader, int lightShader) {
	// Set shaders
	this->shader = basicShader;
	this->lightShader = lightShader;

	// Create vertex array object
    glGenVertexArrays(1, &vertexArrayObj);
    glBindVertexArray(vertexArrayObj);

    // Create buffer
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    float d[] = {
    	// +5.245,+11.785,+0.415,+1,   0, 0,+1,   1,0,0,  1,1,
     //    +5.166,+11.947,+0.356,+1,   0, 0,+1,   1,0,0,  0,1,
     //    +5.256,+11.906,+0.277,+1,   0, 0,+1,   1,0,0,  1,0,
    	-2.40,+1.06,+2.05,+1,   0, 0,+1,   1,0,0,  1,1,
        +1.06,-2.40,-2.48,+1,   0, 0,+1,   1,0,0,  0,1,
        +1.06,+2.51,-2.50,+1,   0, 0,+1,   1,0,0,  1,0,
    };
    numTriangles = 3;
    // Copy data to 
    glBufferData(GL_ARRAY_BUFFER, sizeof(d), d, GL_STATIC_DRAW);

    // Unbind vao and buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void MeshObject::shadowPass() {
	// Set program
    glUseProgram(Light::getShadowShader());

    // Set model matrix
    GameWindow::getRenderer()->camera->setModelMatrix(modelMatrix);

    // Bind vao and buffer
    glBindVertexArray(vertexArrayObj);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    // Set attribute 0 - vertex (vec4)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 12*sizeof(float), (void*)0);

    // Draw cube
    glDrawArrays(GL_TRIANGLES, 0, numTriangles);

    // Disable attributes
    glDisableVertexAttribArray(0);

    // Unbind everything
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);

    // Unbind shader
    glUseProgram(0);
}

void MeshObject::rendererPass(bool useLight) {
	// Set program
    glUseProgram(useLight ? lightShader : shader);

    // Set model matrix
    GameWindow::getRenderer()->camera->setModelMatrix(modelMatrix);

    // Bind vao and buffer
    glBindVertexArray(vertexArrayObj);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    // Set attribute 0 - vertex (vec4)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 12*sizeof(float), (void*)0);
    // Set attribute 1 - normal (vec3)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 12*sizeof(float), (void*)(4*sizeof(float)));
    // Set attribute 2 - color (vec3)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 12*sizeof(float), (void*)(7*sizeof(float)));
    // Set attribute 3 - texture (vec2)
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 12*sizeof(float), (void*)(10*sizeof(float)));   

    // Activate texture
    texture.active();
    // Bind texture
    texture.bind();
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, Light::getShadowTexture());

    // Set uniform value
    int id = glGetUniformLocation(useLight ? lightShader : shader, "text");
    if (id >= 0) glUniform1i(id, 0);
    id = glGetUniformLocation(useLight ? lightShader : shader, "depthText");
    if (id >= 0) glUniform1i(id, 1);

    // Draw cube
    glDrawArrays(GL_TRIANGLES, 0, numTriangles);

    // Disable attributes
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);

    // Unbind everything
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glUseProgram(0);
}

void MeshObject::loadFromFile(char * fileName) {
	// Read from file
	GLMmodel * model = glmReadOBJ(fileName);

	printf("Num mat %d - %s\n", model->nummaterials, model->materials[1].name);

	// Allocate vertices
	float * vertices = new float[model->numvertices * 3];

	for (unsigned int i = 0; i < model->numvertices; i++) {
		vertices[i * 3]     = model->vertices[(i + 1) * 3];
		vertices[i * 3 + 1] = model->vertices[(i + 1) * 3 + 1];
		vertices[i * 3 + 2] = model->vertices[(i + 1) * 3 + 2];
    }


    // Allocate normals
    float * normals = new float[model->numnormals * 3];

    for (unsigned int i = 0; i < model->numnormals; i++) {
		normals[i * 3]     = model->normals[(i + 1) * 3];
		normals[i * 3 + 1] = model->normals[(i + 1) * 3 + 1];
		normals[i * 3 + 2] = model->normals[(i + 1) * 3 + 2];
    }

    // Allocate texcoords
    float * texcoords = new float[model->numtexcoords * 2];

    for (unsigned int i = 0; i < model->numtexcoords; ++i) {
    	texcoords[i * 2]     = model->texcoords[(i + 1) * 2];
    	texcoords[i * 2 + 1] = model->texcoords[(i + 1) * 2 + 1];
    }

	// Allocate data
	data = new float[model->numtriangles * 36];

	// Read triangles
	for (unsigned int i = 0; i < model->numtriangles; i++) {
        GLMtriangle triangle = model->triangles[i];
        for (int j = 0; j < 3; j++) {
        	// Vertex index
        	int vertexIndex = triangle.vindices[j] - 1;

        	// Set vertex
			data[36*i + 12*j]     = vertices[3*vertexIndex];
			data[36*i + 12*j + 1] = vertices[3*vertexIndex + 1];
			data[36*i + 12*j + 2] = vertices[3*vertexIndex + 2];
			data[36*i + 12*j + 3] = 1.0;

			// Normal index
        	int normalIndex = triangle.nindices[j] - 1;

			// Set normal
			data[36*i + 12*j + 4] = normals[3*normalIndex];
			data[36*i + 12*j + 5] = normals[3*normalIndex + 1];
			data[36*i + 12*j + 6] = normals[3*normalIndex + 2];

	        // Set white color and vertex
	        data[36*i + 12*j + 7] = 1.0;
	        data[36*i + 12*j + 8] = 1.0;
	        data[36*i + 12*j + 9] = 1.0;

	        // Texcoords index
	        int texcoordIndex = triangle.tindices[j] - 1;
	        data[36*i + 12*j + 10] = texcoords[2*texcoordIndex];
	        data[36*i + 12*j + 11] = texcoords[2*texcoordIndex + 1];
        }
    }

    // Set num vertices
    numTriangles = model->numtriangles*3;

    // Bind vertex array object
    glBindVertexArray(vertexArrayObj);
    // Bind buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    // Copy data to 
    glBufferData(GL_ARRAY_BUFFER, (model->numtriangles * 36 * sizeof(float)), data, GL_STATIC_DRAW);

    // Unbind vao and buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	// Delete vertices
	delete vertices;
	// Delete normals
	delete normals;

	glmDelete(model);

	ErrCheck("obj");
}


