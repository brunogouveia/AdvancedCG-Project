#include <MeshObject.h>
#include <obj/glm.h>
#include <obj/tiny_obj_loader.h>
#include <iostream>
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
    GameWindow::getRenderer()->camera->setModelMatrix(modelMatrix);

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
    glUseProgram(useLight ? lightShader : shader);

    // Set model matrix
    GameWindow::getRenderer()->camera->setModelMatrix(modelMatrix);

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
    int id = glGetUniformLocation(useLight ? lightShader : shader, "text");
    if (id >= 0) glUniform1i(id, 0);
    id = glGetUniformLocation(useLight ? lightShader : shader, "depthText");
    if (id >= 0) glUniform1i(id, 1);
    id = glGetUniformLocation(useLight ? lightShader : shader, "normalMap");
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
    data = new float[numVertices * 9];
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
    glBufferData(GL_ARRAY_BUFFER, (numVertices * 9 * sizeof(float)), data, GL_STATIC_DRAW);

    // Unbind vao and buffer
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // std::cout << shapes[0].mesh.positions[shapes[0].mesh.indices[0]] << " " << shapes[0].mesh.positions[shapes[0].mesh.indices[1]] << std::endl;

    // // for (int i = 0; i < shapes.size(); ++i) {
    // //     // Create new shape
    // //     MeshShape shape;

    // //     // Get texture name
    // //     std::string textureName = materials[shapes[i].mesh.material_ids[i]].diffuse_texname;
    // //     std::string normalMapName = materials[shapes[i].mesh.material_ids[i]].normal_texname;

    // //     // Set texture
    // //     shape.texture = Texture(GL_TEXTURE0, textureName);
    // //     // Set normalMap
    // //     shape.normalMap = Texture(GL_TEXTURE2, normalMapName);

        
    // //     // Add to vector
    // //     meshShapes.push_back(shape);
    // // }

    // // for (size_t i = 0; i < materials.size(); i++) {
    // //     printf("material[%ld].name = %s\n", i, materials[i].name.c_str());
    // //     printf("  material.Ka = (%f, %f ,%f)\n", materials[i].ambient[0], materials[i].ambient[1], materials[i].ambient[2]);
    // //     printf("  material.Kd = (%f, %f ,%f)\n", materials[i].diffuse[0], materials[i].diffuse[1], materials[i].diffuse[2]);
    // //     printf("  material.Ks = (%f, %f ,%f)\n", materials[i].specular[0], materials[i].specular[1], materials[i].specular[2]);
    // //     printf("  material.Tr = (%f, %f ,%f)\n", materials[i].transmittance[0], materials[i].transmittance[1], materials[i].transmittance[2]);
    // //     printf("  material.Ke = (%f, %f ,%f)\n", materials[i].emission[0], materials[i].emission[1], materials[i].emission[2]);
    // //     printf("  material.Ns = %f\n", materials[i].shininess);
    // //     printf("  material.Ni = %f\n", materials[i].ior);
    // //     printf("  material.dissolve = %f\n", materials[i].dissolve);
    // //     printf("  material.illum = %d\n", materials[i].illum);
    // //     printf("  material.map_Ka = %s\n", materials[i].ambient_texname.c_str());
    // //     printf("  material.map_Kd = %s\n", materials[i].diffuse_texname.c_str());
    // //     printf("  material.map_Ks = %s\n", materials[i].specular_texname.c_str());
    // //     printf("  material.map_Ns = %s\n", materials[i].normal_texname.c_str());
    // //     std::map<std::string, std::string>::const_iterator it(materials[i].unknown_parameter.begin());
    // //     std::map<std::string, std::string>::const_iterator itEnd(materials[i].unknown_parameter.end());
    // //     for (; it != itEnd; it++) {
    // //         printf("  material.%s = %s\n", it->first.c_str(), it->second.c_str());
    // //     }
    // //     printf("\n");
    // // }

    // // Read from file
    // GLMmodel * model = glmReadOBJ(fileName);

    // std::cout << numVertices/3 << " " << model->numtriangles << std::endl;
    // // printf("Num mat %d %d %d %d %d\n", model->nummaterials, model->numvertices, model->numnormals, model->numtexcoords, model->numtriangles);
    // // // for (int i = 0; i < model->nummaterials; ++i) {
    // // //     GLMmaterial materialOBJ = model->materials[i];

    // // //     printf("%s\n", materialOBJ.name);
    // // //     printf("Emmissive %f %f %f\n", materialOBJ.emmissive[0], materialOBJ.emmissive[1], materialOBJ.emmissive[2]);
    // // //     printf("Ambient   %f %f %f\n", materialOBJ.ambient[0], materialOBJ.ambient[1], materialOBJ.ambient[2]);
    // // //     printf("Diffuse   %f %f %f\n", materialOBJ.diffuse[0], materialOBJ.diffuse[1], materialOBJ.diffuse[2]);
    // // //     printf("Specular  %f %f %f\n", materialOBJ.specular[0], materialOBJ.specular[1], materialOBJ.specular[2]);
    // // //     printf("Shininess %f\n", materialOBJ.shininess);
    // // // }
    // // GLMgroup * group = model->groups;
    // // while (group)
    // // {
    // //     GLMmaterial materialOBJ = model->materials[group->material];

    // //     printf("%s\n", group->name);
    // //     printf("Emmissive %f %f %f\n", materialOBJ.emmissive[0], materialOBJ.emmissive[1], materialOBJ.emmissive[2]);
    // //     printf("Ambient   %f %f %f\n", materialOBJ.ambient[0], materialOBJ.ambient[1], materialOBJ.ambient[2]);
    // //     printf("Diffuse   %f %f %f\n", materialOBJ.diffuse[0], materialOBJ.diffuse[1], materialOBJ.diffuse[2]);
    // //     printf("Specular  %f %f %f\n", materialOBJ.specular[0], materialOBJ.specular[1], materialOBJ.specular[2]);
    // //     printf("Shininess %f\n", materialOBJ.shininess);

    // //     group = group->next;
    // // }

    // // Allocate vertices
    // float * vertices = new float[model->numvertices * 3];

    // for (unsigned int i = 0; i < model->numvertices; i++) {
    //     vertices[i * 3]     = model->vertices[(i + 1) * 3];
    //     vertices[i * 3 + 1] = model->vertices[(i + 1) * 3 + 1];
    //     vertices[i * 3 + 2] = model->vertices[(i + 1) * 3 + 2];
    // }


    // // Allocate normals
    // float * normals = new float[model->numnormals * 3];

    // for (unsigned int i = 0; i < model->numnormals; i++) {
    //     normals[i * 3]     = model->normals[(i + 1) * 3];
    //     normals[i * 3 + 1] = model->normals[(i + 1) * 3 + 1];
    //     normals[i * 3 + 2] = model->normals[(i + 1) * 3 + 2];
    // }

    // // Allocate texcoords
    // float * texcoords = new float[model->numtexcoords * 2];

    // for (unsigned int i = 0; i < model->numtexcoords; ++i) {
    //     texcoords[i * 2]     = model->texcoords[(i + 1) * 2];
    //     texcoords[i * 2 + 1] = model->texcoords[(i + 1) * 2 + 1];
    // }

    // // Allocate data
    // data = new float[model->numtriangles * 36];

    // // Read triangles
    // for (unsigned int i = 0; i < model->numtriangles; i++) {
    //     GLMtriangle triangle = model->triangles[i];
    //     for (int j = 0; j < 3; j++) {
    //         // Vertex index
    //         int vertexIndex = triangle.vindices[j] - 1;

    //         // Set vertex
    //         data[36*i + 12*j]     = vertices[3*vertexIndex];
    //         data[36*i + 12*j + 1] = vertices[3*vertexIndex + 1];
    //         data[36*i + 12*j + 2] = vertices[3*vertexIndex + 2];
    //         data[36*i + 12*j + 3] = 1.0;

    //         // Normal index
    //         if (model->numnormals > 0) {
    //             int normalIndex = triangle.nindices[j] - 1;

    //             // Set normal
    //             data[36*i + 12*j + 4] = normals[3*normalIndex];
    //             data[36*i + 12*j + 5] = normals[3*normalIndex + 1];
    //             data[36*i + 12*j + 6] = normals[3*normalIndex + 2];

    //             // Set white color and vertex
    //             data[36*i + 12*j + 7] = 1.0;
    //             data[36*i + 12*j + 8] = 1.0;
    //             data[36*i + 12*j + 9] = 1.0;
    //         }

    //         // Texcoords index
    //         if (model->numtexcoords > 0) {
    //             int texcoordIndex = triangle.tindices[j] - 1;
    //             data[36*i + 12*j + 10] = texcoords[2*texcoordIndex];
    //             data[36*i + 12*j + 11] = texcoords[2*texcoordIndex + 1];
    //         } else {
    //             data[36*i + 12*j + 10] = 0.0;
    //             data[36*i + 12*j + 11] = 0.0;
    //         }
    //     }

    //     // If obj has no normals, we need to compute them
    //     if (model->numnormals == 0) {
    //         // Vectors of triangle
    //         glm::vec3 a = glm::vec3(data[36*i + 12] - data[36*i], data[36*i + 13] - data[36*i + 1], data[36*i + 14] - data[36*i + 2]);
    //         glm::vec3 b = glm::vec3(data[36*i + 24] - data[36*i], data[36*i + 25] - data[36*i + 1], data[36*i + 26] - data[36*i + 2]);

    //         // Compute normal
    //         glm::vec3 n = glm::normalize(glm::cross(a,b));

    //         // Copy normal to data
    //         for (int j = 0; j < 3; ++j) {
    //             data[36*i + 12*j + 4] = n[0];
    //             data[36*i + 12*j + 5] = n[1];
    //             data[36*i + 12*j + 6] = n[2];
    //         }
    //     }
    // }

    // // Set num vertices
    // numVertices = model->numtriangles*3;

    // // Bind vertex array object
    // glBindVertexArray(vertexArrayObj);
    // // Bind buffer
    // glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    // // Copy data to 
    // glBufferData(GL_ARRAY_BUFFER, (model->numtriangles * 36 * sizeof(float)), data, GL_STATIC_DRAW);

    // // Unbind vao and buffer
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);

    // // Delete vertices
    // delete vertices;
    // // Delete normals
    // delete normals;

    // glmDelete(model);

    ErrCheck("obj");
}


MeshShape::MeshShape() : texture(GL_TEXTURE0), normalMap(GL_TEXTURE2, true) {
}