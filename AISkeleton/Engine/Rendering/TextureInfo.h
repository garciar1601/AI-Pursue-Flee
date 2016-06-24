#ifndef TEXTURE_INFO_H
#define TEXTURE_INFO_H
#include <Qt\qimage.h>

struct TextureInfo{
	GLuint location;
	GLuint textureLocation;
	TextureInfo() : location(0), textureLocation(0) {}
};

#endif