#ifndef GEOMETRY_INFO_H
#define GEOMETRY_INFO_H
#include <GL\glew.h>
class VertexLayoutInfo;

class GeometryInfo{
	GLuint vertexArrayObjectID;

	GLuint indexByteOffset;
	GLuint numIndices;
	GLuint indiceDataType;
	GLuint indexingMode;

	bool isAvailable;
	friend class Renderer;
public:
	GeometryInfo() : isAvailable(true) {}
};

#endif