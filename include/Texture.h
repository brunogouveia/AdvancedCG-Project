#ifndef TEXTURE_H__
#define TEXTURE_H__ 

#include <CSCIx239.h>

class Texture
{
private:
	int textureUnit;
	int texture;

	// Static attributes
	static int defaultTexture;
public:
	Texture(int textureUnit, char * fileName);
	~Texture();
	
	void active() const;

	// Static methods
	static int getDefaultTexture();
};

#endif