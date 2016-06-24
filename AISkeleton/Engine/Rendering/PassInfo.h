#ifndef PASS_INFO_H
#define PASS_INFO_H
#include <Camera.h>
#include <Assert.h>
class PassInfo
{
	static const GLuint MAX_RENDERABLES = 2000;
	Renderable* renderables[MAX_RENDERABLES];
	GLuint numRenderables;
	friend class Renderer;
	friend class MeGame;
public:
	Camera camera;
	PassInfo() : numRenderables(0) {}
	inline void addRenderable(Renderable* renderable);
};

void PassInfo::addRenderable(Renderable* renderable){
	ASSERT(numRenderables < MAX_RENDERABLES);
	renderables[numRenderables++] = renderable;
}

#endif