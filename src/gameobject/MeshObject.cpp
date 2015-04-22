#include <MeshObject.h>
#include <obj/glm.h>
#include <obj/tiny_obj_loader.h>
#include <iostream>
#include <GameWindow.h>

MeshObject::MeshObject(glm::mat4 modelMatrix) {
	vertexArrayObj = -1;
	vertexBuffer = -1;
	numVertices = -1;
	this->modelMatrix = modelMatrix;
}

MeshObject::~MeshObject() {

}

void MeshObject::init(int basicShader, int lightShader) {
    // Set shaders
    this->basicShader = basicShader;
    this->lightShader = lightShader;

    // Create vertex array object
    glGenVertexArrays(1, &vertexArrayObj);
    glBindVertexArray(vertexArrayObj);

    // Create buffer
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    float d[] = {
        0,0,0,+1,   0, 0,+1,   1,1,
        0,0,0,+1,   0, 0,+1,   0,1,
        0,0,0,+1,   0, 0,+1,   1,0
    };
    numVertices = 3;
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
    GameWindow::getRenderer()->getCamera()->setModelMatrix(modelMatrix);

    // Bind vao and buffer
    glBindVertexArray(vertexArrayObj);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    // Set attribute 0 - vertex (vec4)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 9*sizeof(float), (void*)0);

    // Draw cube
    glDrawArrays(GL_TRIANGLES, 0, numVertices);

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
    glUseProgram(useLight ? lightShader : basicShader);

    // Set model matrix
    GameWindow::getRenderer()->getCamera()->setModelMatrix(modelMatrix);

    // Bind vao and buffer
    glBindVertexArray(vertexArrayObj);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    // Set attribute 0 - vertex (vec4)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 9*sizeof(float), (void*)0);
    // Set attribute 1 - normal (vec3)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (void*)(4*sizeof(float)));
    // Set attribute 2 - texture (vec2)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (void*)(7*sizeof(float)));

    // Activate texture
    texture.active();
    // Bind texture
    texture.bind();
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, Light::getShadowTexture());
    glTexGendv(GL_S,GL_EYE_PLANE,Light::Svec);
    glTexGendv(GL_T,GL_EYE_PLANE,Light::Tvec);
    glTexGendv(GL_R,GL_EYE_PLANE,Light::Rvec);
    glTexGendv(GL_Q,GL_EYE_PLANE,Light::Qvec);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_COMPARE_MODE,GL_COMPARE_R_TO_TEXTURE);

    // Activate normalMap
    normalMap.active();
    // Bind normal Map
    normalMap.bind();

    // Set uniform value
    int id = glGetUniformLocation(useLight ? lightShader : basicShader, "text");
    if (id >= 0) glUniform1i(id, 0);
    id = glGetUniformLocation(useLight ? lightShader : basicShader, "depthText");
    if (id >= 0) glUniform1i(id, 1);
    id = glGetUniformLocation(useLight ? lightShader : basicShader, "normalMap");
    if (id >= 0) glUniform1i(id, 2);

    // Active material
    material->activeMaterial();

    // Draw cube
    glDrawArrays(GL_TRIANGLES, 0, numVertices);

    // Disable attributes
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    // Unbind everything
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glUseProgram(0);
}

void MeshObject::loadFromFile(char * fileName) {
    // Shapes, materials
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    // Load from file
    std::string err = LoadObj(shapes, materials, fileName);

    // Check for error
    if (!err.empty()) {
        std::cerr << err << std::endl;
        return;
    }

    // Compute size
    numVertices = 0;
    for (unsigned int i = 0; i < shapes.size(); ++i) {
        numVertices += shapes[i].mesh.indices.size();
    }

    // Initialize data
    std::vector<float> data;
    data.reserve(numVertices * 9);
    int curVert = 0; // Current vertex

    // For each shape
    for (unsigned int i = 0; i < shapes.size(); ++i) {
        // For each triangle
        for (unsigned int j = 0; j < shapes[i].mesh.indices.size()/3; ++j) {
            // For each point
            for (unsigned int k = 0; k < 3; ++k) {
                // Point index
                int index = shapes[i].mesh.indices[3*j + k];

                // Set vertex
                data[27 * curVert + 9 * k]     = shapes[i].mesh.positions[3 * index];
                data[27 * curVert + 9 * k + 1] = shapes[i].mesh.positions[3 * index + 1];
                data[27 * curVert + 9 * k + 2] = shapes[i].mesh.positions[3 * index + 2];
                data[27 * curVert + 9 * k + 3] = 1.0;

                // Set normal
                if (shapes[i].mesh.normals.size() > 0) {
                    data[27 * curVert + 9 * k + 4] = shapes[i].mesh.normals[3 * index];
                    data[27 * curVert + 9 * k + 5] = shapes[i].mesh.normals[3 * index + 1];
                    data[27 * curVert + 9 * k + 6] = shapes[i].mesh.normals[3 * index + 2];
                }

                // Set texture
                if (shapes[i].mesh.texcoords.size() > 0) {
                    data[27 * curVert + 9 * k + 7] = shapes[i].mesh.texcoords[2 * index];
                    data[27 * curVert + 9 * k + 8] = shapes[i].mesh.texcoords[2 * index + 1];
                } else {
                    data[27 * curVert + 9 * k + 7] = 0.0;
                    data[27 * curVert + 9 * k + 8] = 0.0;
                }
            }

            // If obj has no normals, we need to compute them
            if (shapes[i].mesh.normals.size() == 0) {
                // Vectors of triangle
                glm::vec3 a = glm::vec3(data[27*curVert + 9] - data[27*curVert], data[27*curVert + 10] - data[27*curVert + 1], data[27*curVert + 11] - data[27*curVert + 2]);
                glm::vec3 b = glm::vec3(data[27*curVert + 18] - data[27*curVert], data[27*curVert + 19] - data[27*curVert + 1], data[27*curVert + 20] - data[27*curVert + 2]);

                // Compute normal
                glm::vec3 n = glm::normalize(glm::cross(a,b));

                // Copy normal to data
                for (int k = 0; k < 3; ++k) {
                    data[27 * curVert + 9 * k + 4] = n[0];
                    data[27 * curVert + 9 * k + 5] = n[1];
                    data[27 * curVert + 9 * k + 6] = n[2];
                }
            }
            // Incremente curVert
            curVert++;                
        }
    }

    // Bind vertex array object
    glBindVertexArray(vertexArrayObj);
    // Bind buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    // Copy data to 
    glBufferData(GL_ARRAY_BUFFER, (numVertices * 9 * sizeof(float)), &data[0], GL_STATIC_DRAW);

    // Unbind vao and buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    ErrCheck("obj");
}
