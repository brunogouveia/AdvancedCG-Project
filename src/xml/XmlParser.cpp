/*
 * XmlParser.cpp
 *
 *  Created on: May 24, 2015
 *      Author: Bruno Gouveia
 */

#include "xml/XmlParser.h"
#include <GameWindow.h>
#include <Input.h>
#include <Cube.h>
#include <Scene.h>
#include <Script.h>
#include <Material.h>
#include <MeshObject.h>

XmlParser::~XmlParser() {
}

void XmlParser::loadFromXml(const char * fileName, int * argc, char ** argv) {
    XmlFile xmlFile(fileName);
    XmlDocument doc;
    doc.parse<0>(xmlFile.data());
    int W, H;

    XmlNode * rtNode = doc.first_node("ge");

    /*** Read window information ***/
    XmlNode * windowNode = rtNode->first_node("window");
    if (windowNode) {
        // Try to read window name
        XmlNode * name;
        xmlElement(name, windowNode);

        // Read width and height
        XmlNode * width, *height;
        xmlElement(width, windowNode);
        xmlElement(height, windowNode);

        W = atoi(width->value());
        H = atoi(height->value());

        // Set window parameters
        GameWindow::setWindow((name) ? name->value() : "GE", W, H);
    } else {
        W = 512;
        H = 512;

        GameWindow::setWindow("GE", W, H);
    }
    // Init gamewindow and light
    GameWindow::init(argc, argv);
    Light::init();
    Material::init();
    Input::init();

    /*** Read Scene ***/
    // Reference to scene node
    XmlNode * sceneNode = rtNode->first_node("scene");
    // Instantiate scene    
    Scene * scene = new Scene();
    // Instantiate renderer
    Renderer * renderer = new Renderer();
    // Set renderer
    GameWindow::setRenderer(renderer);
    renderer->setScene(scene);

    // Create camera
    Camera * camera = new Camera();
    // Set camera
    renderer->setCamera(camera);
    camera->setAspectRatio(W/H);

    // Read Camera
    XmlNode * cameraNode = rtNode->first_node("camera");
    if (cameraNode) {
        // Set position
        XmlNode * position = cameraNode->first_node("position");
        if (position) {
            // Get coordinates
            XmlAttr * x, *y, *z;
            xmlAttribute(x, position);
            xmlAttribute(y, position);
            xmlAttribute(z, position);
            camera->setPosition(atof(x->value()), atof(y->value()), atof(z->value()));
        }

        // Set direction
        XmlNode * direction= cameraNode->first_node("direction");
        if (direction) {
            // Get coordinates
            XmlAttr * x, *y, *z;
            xmlAttribute(x, direction);
            xmlAttribute(y, direction);
            xmlAttribute(z, direction);
            camera->setDirection(atof(x->value()), atof(y->value()),
                    atof(z->value()));
        }


        // Set direction
        XmlNode * up = cameraNode->first_node("up");
        if (up) {
            // Get coordinates
            XmlAttr * x, *y, *z;
            xmlAttribute(x, up);
            xmlAttribute(y, up);
            xmlAttribute(z, up);
            camera->setUp(atof(x->value()), atof(y->value()),
                    atof(z->value()));
        }

        // Read scripts
        XmlNode * scriptsNode = cameraNode->first_node("scripts");
        readScripts(scene, camera, scriptsNode);
    }

    // Light shadow map must be initialized after renderer has a camera
    Light::initShadowMap(renderer->createShaderProg("shaders/shadow.vert", "shaders/shadow.frag"));

    // Read objects
    XmlNode * objectsNode = sceneNode->first_node("objects");
    if (objectsNode) {
        // Read all cubes
        XmlNode * cubeNode = objectsNode->first_node("cube");
        readCubes(renderer, scene, cubeNode);

        // Read all mesh from scene
        XmlNode * meshNode = objectsNode->first_node("mesh");
        readMeshs(renderer, scene, meshNode);
        
    } else {
        printf("No objects\n");
    }

    // Read Lights
    XmlNode * lights = sceneNode->first_node("lights");
    readLights(scene, lights);
}

void XmlParser::readCubes(Renderer * renderer ,Scene * scene, XmlNode * cubeNode) {
    // Read all cubes from scene
    while(cubeNode) {
        // Create cube
        Cube * c = new Cube();
        // Read shaders and init it
        int basicShader = renderer->getDefaultBasicShader();
        int lightShader = renderer->getDefaultLightShader();
        XmlNode * basicShaderNode = cubeNode->first_node("basic-shader");
        XmlNode * lightShaderNode = cubeNode->first_node("light-shader");
        
        // If there is a basic shader node
        if (basicShaderNode) {
            XmlAttr * vert, * frag;
            xmlAttribute(vert, basicShaderNode);
            xmlAttribute(frag, basicShaderNode);
            // Create new shader
            basicShader = renderer->createShaderProg(vert->value(), frag->value());
        }

        // If there is a light shader node
        if (lightShaderNode) {
            XmlAttr * vert, * frag;
            xmlAttribute(vert, lightShaderNode);
            xmlAttribute(frag, lightShaderNode);
            // Create new shader
            lightShader = renderer->createShaderProg(vert->value(), frag->value());
        }

        // Init cube
        c->init(basicShader, lightShader);
        
        // Try to read model
        readModel(c, cubeNode->first_node("model"));


        // Add c to scene
        scene->addGameObject(c);

        // Set texture
        XmlNode * texture = cubeNode->first_node("texture");
        if (texture) {
            // Get file name
            XmlAttr * file;
            xmlAttribute(file, texture);

            // Create texture
            Texture texture(GL_TEXTURE0, std::string(file->value()));

            // Set texture
            c->setTexture(texture);
        }

        // Set material
        XmlNode * material = cubeNode->first_node("material");
        if (material) {
            // Create material
            Material * m = new Material();

            // Set emissive
            XmlNode * emissive = material->first_node("emissive");
            if (emissive) {
                XmlAttr * r, *g, *b;
                xmlAttribute(r, emissive);
                xmlAttribute(g, emissive);
                xmlAttribute(b, emissive);
                // Set diffuse
                m->setEmissive(atof(r->value()), atof(g->value()),
                        atof(b->value()));
            }

            // Read ambient
            XmlNode * ambient = material->first_node("ambient");
            if (ambient) {
                XmlAttr * r, *g, *b;
                xmlAttribute(r, ambient);
                xmlAttribute(g, ambient);
                xmlAttribute(b, ambient);
                // Set diffuse
                m->setAmbient(atof(r->value()), atof(g->value()),
                        atof(b->value()));
            }

            // Read diffuse
            XmlNode * diffuse = material->first_node("diffuse");
            if (diffuse) {
                XmlAttr * r, *g, *b;
                xmlAttribute(r, diffuse);
                xmlAttribute(g, diffuse);
                xmlAttribute(b, diffuse);
                // Set diffuse
                m->setDiffuse(atof(r->value()), atof(g->value()),
                        atof(b->value()));
            }

            // Read specular
            XmlNode * specular = material->first_node("specular");
            if (specular) {
                XmlAttr * r, *g, *b;
                xmlAttribute(r, specular);
                xmlAttribute(g, specular);
                xmlAttribute(b, specular);
                // Set diffuse
                m->setSpecular(atof(r->value()), atof(g->value()),
                        atof(b->value()));
            }

            // Read specular
            XmlNode * shininess = material->first_node("shininess");
            if (shininess) {
                XmlAttr * value;
                xmlAttribute(value, shininess);
                // Set diffuse
                m->setShininess(atof(value->value()));
            }

            // Set material
            c->setMaterial(m);
        }

        // Read Scripts
        XmlNode * scripts = cubeNode->first_node("scripts");
        readScripts(scene, c, scripts);

        // Get next cube
        cubeNode = cubeNode->next_sibling("cube");
    }
}

void XmlParser::readMeshs(Renderer * renderer ,Scene * scene, XmlNode * meshNode) {
    // Read mesh
    while(meshNode) {
        // Create cube
        glm::mat4 id;
        MeshObject * mesh = new MeshObject(id);
        // Read shaders and init it
        int basicShader = renderer->getDefaultBasicShader();
        int lightShader = renderer->getDefaultLightShader();
        XmlNode * basicShaderNode = meshNode->first_node("basic-shader");
        XmlNode * lightShaderNode = meshNode->first_node("light-shader");
        
        // If there is a basic shader node
        if (basicShaderNode) {
            XmlAttr * vert, * frag;
            xmlAttribute(vert, basicShaderNode);
            xmlAttribute(frag, basicShaderNode);
            // Create new shader
            basicShader = renderer->createShaderProg(vert->value(), frag->value());
        }

        // If there is a light shader node
        if (lightShaderNode) {
            XmlAttr * vert, * frag;
            xmlAttribute(vert, lightShaderNode);
            xmlAttribute(frag, lightShaderNode);
            // Create new shader
            lightShader = renderer->createShaderProg(vert->value(), frag->value());
        }

        // Init mesh
        mesh->init(basicShader, lightShader);

        // Load file (it must exist)
        XmlAttr * file;
        xmlAttribute(file, meshNode);
        mesh->loadFromFile(file->value());
        
        // Try to read model
        readModel(mesh, meshNode->first_node("model"));

        // Add c to scene
        scene->addGameObject(mesh);

        // Set texture
        XmlNode * textureNode = meshNode->first_node("texture");
        if (textureNode) {
            // Get file name
            XmlAttr * file;
            xmlAttribute(file, textureNode);

            // Create texture
            Texture texture(GL_TEXTURE0, std::string(file->value()));

            // Set texture
            mesh->setTexture(texture);
        }

        // Set normal map
        XmlNode * normalMapNode = meshNode->first_node("normal-map");
        if (normalMapNode) {
            // Get file name
            XmlAttr * file;
            xmlAttribute(file, normalMapNode);

            // Create texture
            Texture normalMap(GL_TEXTURE2, std::string(file->value()));

            // Set texture
            mesh->setNormalMap(normalMap);
        }

        // Set material
        XmlNode * material = meshNode->first_node("material");
        if (material) {
            // Create material
            Material * m = new Material();

            // Set emissive
            XmlNode * emissive = material->first_node("emissive");
            if (emissive) {
                XmlAttr * r, *g, *b;
                xmlAttribute(r, emissive);
                xmlAttribute(g, emissive);
                xmlAttribute(b, emissive);
                // Set diffuse
                m->setEmissive(atof(r->value()), atof(g->value()),
                        atof(b->value()));
            }

            // Read ambient
            XmlNode * ambient = material->first_node("ambient");
            if (ambient) {
                XmlAttr * r, *g, *b;
                xmlAttribute(r, ambient);
                xmlAttribute(g, ambient);
                xmlAttribute(b, ambient);
                // Set diffuse
                m->setAmbient(atof(r->value()), atof(g->value()),
                        atof(b->value()));
            }

            // Read diffuse
            XmlNode * diffuse = material->first_node("diffuse");
            if (diffuse) {
                XmlAttr * r, *g, *b;
                xmlAttribute(r, diffuse);
                xmlAttribute(g, diffuse);
                xmlAttribute(b, diffuse);
                // Set diffuse
                m->setDiffuse(atof(r->value()), atof(g->value()),
                        atof(b->value()));
            }

            // Read specular
            XmlNode * specular = material->first_node("specular");
            if (specular) {
                XmlAttr * r, *g, *b;
                xmlAttribute(r, specular);
                xmlAttribute(g, specular);
                xmlAttribute(b, specular);
                // Set diffuse
                m->setSpecular(atof(r->value()), atof(g->value()),
                        atof(b->value()));
            }

            // Read specular
            XmlNode * shininess = material->first_node("shininess");
            if (shininess) {
                XmlAttr * value;
                xmlAttribute(value, shininess);
                // Set diffuse
                m->setShininess(atof(value->value()));
            }

            // Set material
            mesh->setMaterial(m);
        }

        // Read Scripts
        XmlNode * scripts = meshNode->first_node("scripts");
        readScripts(scene, mesh, scripts);

        // Get next cube
        meshNode = meshNode->next_sibling("mesh");
    }
}

void XmlParser::readLights(Scene * scene, XmlNode * lights) {
    if (lights) {
        // Read all lights from scene
        XmlNode * light = lights->first_node("light");
        while (light) {
            // Create light
            Light * l = new Light();

            // Get position from xml
            XmlNode * position;
            xmlElement(position, light);
            // Get coordinates
            XmlAttr * x, * y, * z;
            xmlAttribute(x, position);
            xmlAttribute(y, position);
            xmlAttribute(z, position);
            // Set position
            l->setPosition(atof(x->value()), atof(y->value()), atof(z->value()));

            // Get direction from xml
            XmlNode * direction;
            xmlElement(direction, light);
            // Get coordinates
            xmlAttribute(x, direction);
            xmlAttribute(y, direction);
            xmlAttribute(z, direction);
            // Set position
            l->setDirection(atof(x->value()), atof(y->value()), atof(z->value()));

            // Check angle
            XmlNode * angle = light->first_node("angle");
            if (angle) {
                XmlAttr * value;
                xmlAttribute(value, angle);
                l->setLightAngle(atof(value->value()));
            }

            // Try to get diffuse
            XmlNode * diffuse = light->first_node("diffuse");
            if (diffuse) {
                // Get coordinates
                XmlAttr * r, * g, * b;
                xmlAttribute(r, diffuse);
                xmlAttribute(g, diffuse);
                xmlAttribute(b, diffuse);
                // Set diffuse
                l->setDiffuse(atof(r->value()), atof(g->value()), atof(b->value()));
            }

            // Try to get specular
            XmlNode * specular = light->first_node("specular");
            if (specular) {
                // Get coordinates
                XmlAttr * r, * g, * b;
                xmlAttribute(r, specular);
                xmlAttribute(g, specular);
                xmlAttribute(b, specular);
                // Set specular
                l->setSpecular(atof(r->value()), atof(g->value()), atof(b->value()));
            }

            // Read scripts
            XmlNode * scripts = light->first_node("scripts");
            readScripts(scene, l, scripts);

            // Add to scene
            scene->addLight(l);

            // Get next light
            light = light->next_sibling("light");
        }
    } else {
        printf("No lights\n");
    }
}

void XmlParser::readScripts(Scene * scene, Transform * transform, XmlNode * scripts) {
    if (scripts) {
        // Get each script
        XmlNode * script = scripts->first_node("script");
        while (script) {
            XmlAttr * name;
            xmlAttribute(name, script);

            // Crate script and bind it to transform
            scene->addScript(Script::GetScriptByName(transform, name->value()));

            // Get next script
            script = script->next_sibling("script");
        }
    }
}


void XmlParser::readModel(Transform * transform, XmlNode * modelNode) {
    // Create model
    glm::mat4 model;

    if (modelNode) {
        // Get each transformation
        XmlNode * transformNode = modelNode->first_node();

        while (transformNode) {
            std::string type(transformNode->name());

            // Translate
            if (type == "translate") {
                XmlAttr * x, * y, * z;
                xmlAttribute(x, transformNode);
                xmlAttribute(y, transformNode);
                xmlAttribute(z, transformNode);

                // Apply transformation
                transform->translate(atof(x->value()), atof(y->value()), atof(z->value()));
            }
            // Rotate
            else if (type == "rotate") {
                // Read angle
                XmlAttr * angle;
                xmlAttribute(angle, transformNode);

                // Check normal
                glm::vec3 normal(0, 1, 0);
                XmlNode * normalNode = transformNode->first_node("normal");
                if (normalNode) {
                    XmlAttr * x, * y, * z;
                    xmlAttribute(x, normalNode);
                    xmlAttribute(y, normalNode);
                    xmlAttribute(z, normalNode);

                    // Set new normal
                    normal = glm::vec3(atof(x->value()), atof(y->value()), atof(z->value()));
                }

                // Apply transformation
                transform->rotate(atof(angle->value()), normal);
            }
            // Scale
            else if (type == "scale") {
                XmlAttr * x, * y, * z;
                xmlAttribute(x, transformNode);
                xmlAttribute(y, transformNode);
                xmlAttribute(z, transformNode);

                // Apply transformation
                transform->scale(atof(x->value()), atof(y->value()), atof(z->value()));
            }

            // Get next transformation
            transformNode = transformNode->next_sibling();
        }
    }
}

