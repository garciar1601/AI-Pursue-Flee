#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <glm.hpp>
#include <Rendering\UniformInfo.h>
class GeometryInfo;
struct ShaderInfo;
struct TextureInfo;

struct Renderable{
	GeometryInfo* what;
	ShaderInfo* how;
	TextureInfo* texture;
	static const unsigned int MAX_UNIFORM_INFOS = 200;
	UniformInfo uniformInfos[MAX_UNIFORM_INFOS];
	unsigned int numUniforms;
	glm::mat4 where;
	bool visible;
	Renderable() : visible(true), numUniforms(0) {}
};

#endif