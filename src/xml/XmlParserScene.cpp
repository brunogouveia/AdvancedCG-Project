/*
 * XmlParser.cpp
 *
 *  Created on: 16/12/2013
 *      Author: Bruno Gouveia
 */

#ifndef __XmlParser_h
#include "XmlParser.h"
#endif
#ifndef __MeshSweeper_h
#include "../MeshSweeper.h"
#endif

XmlParser::~XmlParser() {
}

void XmlParser::loadFromXml(const char * fileName, Scene *& s, Camera *& c, int& W, int& H) {
	XmlFile xmlFile(fileName);
	XmlDocument doc;
	doc.parse<0>(xmlFile.data());

	XmlNode * rtNode = doc.first_node("rt");

	XmlNode * imageNode = rtNode->first_node("image");
	if (imageNode) {
		XmlNode * width, *height;
		xmlElement(width, imageNode);
		xmlElement(height, imageNode);

		W = atoi(width->value());
		H = atoi(height->value());
	} else {
		W = 512;
		H = 512;
	}

	XmlNode * camera = rtNode->first_node("camera");
	if (camera) {
		c = xmlToCamera(camera);
	} else {
		c = new Camera();
		c->setPosition(Vec3(0, 0, 10));
		c->setViewAngle(90);
		c->setHeight(20);
		c->setAspectRatio(1);
		c->setProjectionType(Camera::Perspective);
	}

	XmlNode * scene;
	xmlElement(scene, rtNode);

	s = xmlToScene(scene);
}

Camera * XmlParser::xmlToCamera(XmlNode * camera) {
	Camera * c = new Camera();
	REAL x, y, z;

	//parametros obrigatorios
	XmlNode * position, *to, *up;
	xmlElement(position, camera);
	xmlElement(to, camera);
	xmlElement(up, camera);
	{
		sscanf(position->value(), "%lf %lf %lf", &x, &y, &z);
		c->setPosition(Vec3(x, y, z));
		sscanf(to->value(), "%lf %lf %lf", &x, &y, &z);
		c->setDirectionOfProjection((Vec3(x, y, z) - c->getPosition()));
		sscanf(up->value(), "%lf %lf %lf", &x, &y, &z);
		c->setViewUp(Vec3(x, y, z));
	}

	XmlNode * angle = camera->first_node("angle");
	if (angle) {
		sscanf(angle->value(), "%lf", &x);
		c->setViewAngle(x);
		c->setHeight(2 * tan(Math::toRadians<REAL>(x * 0.5)) * c->getDistance());
	} else {
		c->setViewAngle(90);
		c->setHeight(20);
	}

	XmlNode * aspect = camera->first_node("aspect");
	if (aspect) {
		sscanf(aspect->value(), "%lf:%lf", &x, &y);
		c->setAspectRatio(x / y);
	} else {
		c->setAspectRatio(1);
	}

	XmlNode * projection = camera->first_node("projection");
	if (projection) {
		if (strcmp(projection->value(), "perspective") == 0)
			c->setProjectionType(Camera::Perspective);
		else
			c->setProjectionType(Camera::Parallel);
	} else {
		c->setProjectionType(Camera::Perspective);
	}
	return c;
}

Scene * XmlParser::xmlToScene(XmlNode * scene) {
	Scene * s;
	char * nameScene = (scene->first_attribute("name")) ? scene->first_attribute("name")->value() : 0;
	s = (nameScene != 0) ? new Scene(nameScene) : Scene::New();

	REAL x, y, z;

	XmlNode * background;
	xmlElement(background, scene);
	{
		sscanf(background->value(), "%lf %lf %lf", &x, &y, &z);
		s->backgroundColor = Color(x, y, z);
	}

	XmlNode * ambient;
	xmlElement(ambient, scene);
	{
		sscanf(ambient->value(), "%lf %lf %lf", &x, &y, &z);
		s->ambientLight = Color(x, y, z);
	}

	XmlNode * mesh = scene->first_node("mesh");
	while (mesh) {
		XmlAttr * file;
		xmlAttribute(file, mesh);

		TriangleMeshShape * shape = MeshSweeper::loadFromObj(file->value());
		xmlCheckTransMaterials(shape, mesh);

		s->addActor(new Actor(*shape));

		mesh = mesh->next_sibling("mesh");
	}

	mesh = scene->first_node("sphere");
	while (mesh) {
		TriangleMeshShape * shape = xmlToSphere(mesh);

		s->addActor(new Actor(*shape));

		mesh = mesh->next_sibling("sphere");
	}

	mesh = scene->first_node("box");
	while (mesh) {
		TriangleMeshShape * shape = xmlToBox(mesh);

		s->addActor(new Actor(*shape));

		mesh = mesh->next_sibling("box");
	}

	mesh = scene->first_node("cone");
	while (mesh) {
		TriangleMeshShape * shape = xmlToCone(mesh);

		s->addActor(new Actor(*shape));

		mesh = mesh->next_sibling("cone");
	}

	mesh = scene->first_node("cylinder");
	while (mesh) {
		TriangleMeshShape * shape = xmlToCylinder(mesh);

		s->addActor(new Actor(*shape));

		mesh = mesh->next_sibling("cylinder");
	}

	mesh = scene->first_node("screw");
	while (mesh) {
		TriangleMeshShape * shape = xmlToScrew(mesh);

		s->addActor(new Actor(*shape));

		mesh = mesh->next_sibling("screw");
	}

	XmlNode *light = scene->first_node("light");
	while (light) {
		s->addLight(xmlToLight(light));

		light = light->next_sibling("light");
	}

	return s;
}

Light * XmlParser::xmlToLight(XmlNode * light) {
	Vec3 pos;
	Color color = Color::white;
	int falloff = 0;
	int pontual = 1;

	XmlNode * position;
	xmlElement(position, light);
	sscanf(position->value(), "%lf %lf %lf %d", &pos.x, &pos.y, &pos.z, &pontual);

	XmlNode * node = light->first_node("color");
	if (node)
		sscanf(node->value(), "%lf %lf %lf", &color.r, &color.g, &color.b);

	node = light->first_node("falloff");
	if (node)
		falloff = atoi(node->value());

	Light * l = new Light(pos, color);
	switch (falloff) {
		case 1:
			l->flags.enable(Light::Squared, false);
			l->flags.enable(Light::Linear, true);
			break;
		case 2:
			l->flags.enable(Light::Squared, true);
			l->flags.enable(Light::Linear, false);
			break;
		default:
			l->flags.enable(Light::Squared, false);
			l->flags.enable(Light::Linear, false);

	}
	l->setDirectional((pontual) ? false : true);

	return l;
}

void XmlParser::xmlCheckTransMaterials(TriangleMeshShape * shape, XmlNode * mesh) {
	XmlNode * node;
	node = mesh->first_node("transform");
	if (node) {
		shape->transform(xmlToTransf(node));
	}

	node = mesh->first_node("material");
	if (node) {
		shape->setMaterial(*xmlToMaterial(node));
	}
}

TriangleMeshShape* XmlParser::xmlToSphere(XmlNode * sphere) {
	Vec3 cen;
	REAL rad;
	int mer;

	TriangleMeshShape * shape;
	XmlNode * center, *radius, *meridians;

	xmlElement(center, sphere);
	xmlElement(radius, sphere);

	sscanf(center->value(), "%lf %lf %lf", &cen.x, &cen.y, &cen.z);
	rad = atof(radius->value());

	meridians = sphere->first_node("meridians");
	if (meridians) {
		mer = atoi(meridians->value());

		shape = MeshSweeper::makeSphere(cen, rad, mer);
	} else {
		shape = MeshSweeper::makeSphere(cen, rad);
	}

	xmlCheckTransMaterials(shape, sphere);

	return shape;
}

TriangleMeshShape* XmlParser::xmlToCone(XmlNode * cone) {
	Vec3 cen(0, 0, 0);
	Vec3 norm(0, 1, 0);
	REAL rad = 1;
	REAL heig = 2;
	int seg = 16;

	TriangleMeshShape * shape;
	XmlNode * node;

	node = cone->first_node("center");
	if (node)
		sscanf(node->value(), "%lf %lf %lf", &cen.x, &cen.y, &cen.z);

	node = cone->first_node("normal");
	if (node)
		sscanf(node->value(), "%lf %lf %lf", &norm.x, &norm.y, &norm.z);

	node = cone->first_node("radius");
	if (node)
		rad = atof(node->value());

	node = cone->first_node("height");
	if (node)
		heig = atof(node->value());

	node = cone->first_node("segments");
	if (node)
		seg = atoi(node->value());

	cen.print("Center: ", stdout);
	norm.print("Normal: ", stdout);
	printf("%lf %lf %d\n", rad, heig, seg);

	shape = MeshSweeper::makeCone(cen, norm, rad, heig, seg);
	xmlCheckTransMaterials(shape, cone);
	return shape;
}

TriangleMeshShape* XmlParser::xmlToCylinder(XmlNode * cylinder) {
	Vec3 cen(0, 0, 0);
	Vec3 norm(0, 1, 0);
	REAL rad = 1;
	REAL heig = 2;
	int seg = 16;

	TriangleMeshShape * shape;
	XmlNode * node;

	node = cylinder->first_node("center");
	if (node)
		sscanf(node->value(), "%lf %lf %lf", &cen.x, &cen.y, &cen.z);

	node = cylinder->first_node("normal");
	if (node)
		sscanf(node->value(), "%lf %lf %lf", &norm.x, &norm.y, &norm.z);

	node = cylinder->first_node("radius");
	if (node)
		rad = atof(node->value());

	node = cylinder->first_node("height");
	if (node)
		heig = atof(node->value());

	node = cylinder->first_node("segments");
	if (node)
		seg = atoi(node->value());

	shape = MeshSweeper::makeCylinder(cen, rad, norm, heig, seg);
	xmlCheckTransMaterials(shape, cylinder);
	return shape;
}

TriangleMeshShape* XmlParser::xmlToScrew(XmlNode * screw) {
	Vec3 cen(2, 0, 0);
	Vec3 first(1, 0, 0);
	Vec3 norm(0, 0, 1);
	REAL distance = 1;
	REAL angle = 360;
	REAL delta = 0;
	REAL heig = 2;
	int steps = 36;

	TriangleMeshShape * shape;
	XmlNode * node;

	node = screw->first_node("center");
	if (node)
		sscanf(node->value(), "%lf %lf %lf", &cen.x, &cen.y, &cen.z);

	node = screw->first_node("first");
	if (node)
		sscanf(node->value(), "%lf %lf %lf", &first.x, &first.y, &first.z);

	node = screw->first_node("normal");
	if (node)
		sscanf(node->value(), "%lf %lf %lf", &norm.x, &norm.y, &norm.z);

	node = screw->first_node("distance");
	if (node)
		distance = atof(node->value());

	node = screw->first_node("angle");
	if (node)
		angle = atof(node->value());

	node = screw->first_node("delta");
	if (node)
		delta = atof(node->value());

	node = screw->first_node("height");
	if (node)
		heig = atof(node->value());

	node = screw->first_node("steps");
	if (node)
		steps = atoi(node->value());

	shape = MeshSweeper::makeScrew(cen, first, norm, distance, angle, heig, delta, steps);
	xmlCheckTransMaterials(shape, screw);
	return shape;
}

TriangleMeshShape* XmlParser::xmlToBox(XmlNode * box) {
	Vec3 cen(0, 0, 0);
	Vec3 ori(0, 0, 0);
	Vec3 sca(1, 1, 1);

	TriangleMeshShape * shape;
	XmlNode * node;

	node = box->first_node("center");
	if (node)
		sscanf(node->value(), "%lf %lf %lf", &cen.x, &cen.y, &cen.z);

	node = box->first_node("orientation");
	if (node)
		sscanf(node->value(), "%lf %lf %lf", &ori.x, &ori.y, &ori.z);

	node = box->first_node("scale");
	if (node)
		sscanf(node->value(), "%lf %lf %lf", &sca.x, &sca.y, &sca.z);

	shape = MeshSweeper::makeBox(cen, ori, sca);
	xmlCheckTransMaterials(shape, box);
	return shape;
}

Transf3 XmlParser::xmlToTransf(XmlNode * transformations) {
	XmlNode * transformation = transformations->first_node();

	Transf3 t;
	t.identity();
	while (transformation) {
		char * type = transformation->name();

		if (strcmp(type, "scale") == 0) {
			Transf3 temp;
			temp.scale(atof(transformation->value()));
			t.compose(temp);
		} else if (strcmp(type, "rotation") == 0) {
			XmlNode * axis, *angle;
			xmlElement(axis, transformation);
			xmlElement(angle, transformation);

			Vec3 vec;
			sscanf(axis->value(), "%lf %lf %lf", &vec.x, &vec.y, &vec.z);

			Transf3 temp;
			temp.rotation(vec, Math::toRadians<REAL>(atof(angle->value())));
			t.compose(temp);
		} else if (strcmp(type, "translation") == 0) {
			Transf3 temp;

			Vec3 vec;
			sscanf(transformation->value(), "%lf %lf %lf", &vec.x, &vec.y, &vec.z);
			temp.translation(vec);
			t.compose(temp);
		}
		transformation = transformation->next_sibling();
	}
	return t;
}

Material* XmlParser::xmlToMaterial(XmlNode * material) {
	Material * m = MaterialFactory::New();

	XmlNode * node;
	Color color;

	node = material->first_node("ambient");
	if (node) {
		sscanf(node->value(), "%lf %lf %lf", &color.r, &color.g, &color.b);
		m->surface.ambient = color;
	}

	node = material->first_node("diffuse");
	if (node) {
		sscanf(node->value(), "%lf %lf %lf", &color.r, &color.g, &color.b);
		m->surface.diffuse = color;
	}

	node = material->first_node("spot");
	if (node) {
		sscanf(node->value(), "%lf %lf %lf", &color.r, &color.g, &color.b);
		m->surface.spot = color;
	}

	node = material->first_node("spot");
	if (node) {
		m->surface.shine = atof(node->value());
	}

	node = material->first_node("specular");
	if (node) {
		sscanf(node->value(), "%lf %lf %lf", &color.r, &color.g, &color.b);
		m->surface.specular = color;
	}
	return m;
}
