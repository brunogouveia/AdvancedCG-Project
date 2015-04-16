#ifndef XMLPARSERSCENE_H__
#define XMLPARSERSCENE_H__

#include <Scene.h>
#ifndef RAPIDXML_HPP_INCLUDED
#include "rapidxml.hpp"
#endif
#ifndef RAPIDXML_UTILS_HPP_INCLUDED
#include "rapidxml_utils.hpp"
#endif

using namespace Graphics;
using namespace rapidxml;

#define xmlElement(node, parent) \
	node = parent->first_node(#node);    \
	if (!node) {\
		printf("Missing '"#node"' into %s\n", parent->name());\
		exit(1);\
	}

#define xmlAttribute(attribute, node) \
	attribute = node->first_attribute(#attribute);    \
	if (!attribute) {\
		printf("Missing '"#attribute"' into %s\n", node->name());\
		exit(1);\
	}

typedef file<> XmlFile;
typedef xml_document<> XmlDocument;
typedef xml_node<> XmlNode;
typedef xml_attribute<> XmlAttr;

class XmlParser {
	public:
		virtual ~XmlParser();

		static void loadFromXml(const char * fileName,Scene *& s, Camera *& c, int& W, int& H);
		static Camera * xmlToCamera(XmlNode * camera);
		static Scene * xmlToScene(XmlNode * scene);
		static Light * xmlToLight(XmlNode * light);
		static void xmlCheckTransMaterials(TriangleMeshShape * shape, XmlNode * node);

		static TriangleMeshShape* xmlToSphere(XmlNode * sphere);
		static TriangleMeshShape* xmlToBox(XmlNode * box);
		static TriangleMeshShape* xmlToCone(XmlNode * cone);
		static TriangleMeshShape* xmlToCylinder(XmlNode * cylinder);
		static TriangleMeshShape* xmlToScrew(XmlNode * screw);

		static Transf3 xmlToTransf(XmlNode * transformations);
		static Material* xmlToMaterial(XmlNode * material);
	private:
		XmlParser();
};

#endif /* XMLPARSERSCENE_H__ */
