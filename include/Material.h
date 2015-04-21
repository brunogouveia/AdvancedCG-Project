#ifndef MATERIAL_H__
#define MATERIAL_H__ 

#include <CSCIx239.h>

class Material
{
private:
	// Static attributes
	static GLuint materialBuffer;
	static int materialBindingPoint;
	static Material defaultMaterial;

	// Material attributes;
	float data[16];
public:
	Material();

	// Static methods
	static void init();
	static void bindMaterialBuffers(int shader);
	static Material * getDefaultMaterial();

	void activeMaterial();

	void setEmissive(float r, float g, float b);
	void setAmbient(float r, float g, float b);
	void setDiffuse(float r, float g, float b);
	void setSpecular(float r, float g, float b);
	void setShininess(float shininess);
};

#endif