#ifndef MYVERTEX_H
#define MYVERTEX_H
#include "glm.hpp"
#include <ExportHeader.h>
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

struct ENGINE_SHARED MyVertex
{
	glm::vec3 position;
	glm::vec4 color;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec3 tangent;
};

#endif