#define GLM_FORCE_RADIANS
#include <Debug\DebugTools\DebugShapes.h>
#include <Rendering\VertexLayoutInfo.h>
#include <Rendering\ShaderInfo.h>
#include <Rendering\Renderer.h>
#include <Rendering\UniformDataTypes.h>
#include <ShapeGenerator.h>
#include <gtx\transform.hpp>
#include <gtx\rotate_vector.hpp>
#include <Qt\qdebug.h>

DebugShapes* DebugShapes::instance = 0;

#ifdef DEBUG_SHAPES
bool DebugShapes::initialize(){
	if(instance != 0){
		return false;
	}
	instance = new DebugShapes;
	instance->addGeometries();
	instance->numGeometries = 0;
	return true;
}

void DebugShapes::sendShader(ShaderInfo* input){
	shader = input;
}

void DebugShapes::addGeometries(){
	GLuint sizes[] = {3, 4, 3, 2 };
	VertexLayoutInfo* vertexLayoutInfo = renderer.addVertexLayoutInfo(sizes, ARRAYSIZE(sizes), Neumont::Vertex::STRIDE);
	Neumont::ShapeData data = Neumont::ShapeGenerator::makeSphere(20);
	geometries[0] =
	renderer.addGeometry(
		data.verts, data.vertexBufferSize(),
		data.indices, data.numIndices, data.indexBufferSize(),
		GL_UNSIGNED_SHORT, GL_TRIANGLES, vertexLayoutInfo);

	data = Neumont::ShapeGenerator::makeCube();
	geometries[1] =
	renderer.addGeometry(
		data.verts, data.vertexBufferSize(),
		data.indices, data.numIndices, data.indexBufferSize(),
		GL_UNSIGNED_SHORT, GL_TRIANGLES, vertexLayoutInfo);

	GLfloat crosshairFloats[] =
	{
		0.0f, 0.0f, -0.5f, // 0
		0.0f, 0.0f, 1.0f,

		0.0f, 0.0f, 0.5f, // 1
		1.0f, 1.0f, 1.0f,

		0.0f, 0.0f, 0.0f, // 2
		0.5f, 0.5f, 1.0f,


		-0.5f, 0.0f, 0.0f, // 3
		1.0f, 0.0f, 0.0f,

		0.5f, 0.0f, 0.0f, // 4
		1.0f, 1.0f, 1.0f,

		0.0f, 0.0f, 0.0f, // 5
		1.0f, 0.5f, 0.5f,


		0.0f, -0.5f, 0.0f, // 6
		0.0f, 1.0f, 0.0f,

		0.0f, 0.5f, 0.0f, // 7
		1.0f, 1.0f, 1.0f,

		0.0f, 0.0f, 0.0f, // 8
		0.5f, 1.0f, 0.5f
	};

	unsigned short indices[] =
	{
		0, 1, 2, 5,
		3, 4, 5, 8,
		6, 7
	};
	GLuint crosshairSizes[] = {3, 3};
	vertexLayoutInfo = renderer.addVertexLayoutInfo(crosshairSizes, ARRAYSIZE(crosshairSizes), 6 * sizeof(float));


	geometries[2] = renderer.addGeometry(crosshairFloats, sizeof(crosshairFloats), 
		indices, sizeof(indices) / sizeof(indices[0]), sizeof(indices), GL_UNSIGNED_SHORT, 
		GL_LINES, vertexLayoutInfo);

	GLfloat vectorFloats[] = {
		0.0f, 0.0f, 0.0f, //0
		1.0f, 1.0f, 1.0f,

		0.0f, 0.75f, 0.0f, //1
		1.0f, 1.0f, 1.0f,

		0.0f, 1.0f, 0.0f, //2
		1.0f, 1.0f, 1.0f
	};

	unsigned short vectorIndices[] =
	{
		0, 1, 2
	};

	geometries[3] = renderer.addGeometry(vectorFloats, sizeof(vectorFloats), 
		vectorIndices, sizeof(vectorIndices) / sizeof(vectorIndices[0]), sizeof(vectorIndices), GL_UNSIGNED_SHORT, 
		GL_LINES, vertexLayoutInfo);
}

Renderable* DebugShapes::addSphere(glm::vec3 location, glm::mat4 rotation, glm::mat4 scale, glm::vec3* color){
	Renderable* ret = renderer.addRenderable(geometries[0], shader, glm::translate(location) * rotation * glm::scale(glm::vec3(0.5f, 0.5f, 0.5f)) * scale);
	renderer.addUniformToRenderable(color, "uniformColor", VEC3, ret);
	return ret;
}

Renderable* DebugShapes::addCube(glm::vec3 location, glm::mat4 rotation, glm::mat4 scale, glm::vec3* color){
	Renderable* ret = renderer.addRenderable(geometries[1], shader, glm::translate(location) * rotation * glm::scale(glm::vec3(0.5f, 0.5f, 0.5f)) * scale);
	renderer.addUniformToRenderable(color, "uniformColor", VEC3, ret);
	return ret;
}

Renderable* DebugShapes::addVector(glm::vec3 start, glm::vec3 facing, glm::mat4 scale, ShaderInfo* passThrough){
	glm::vec3 direction = glm::normalize(facing - start);
	Renderable* ret = renderer.addRenderable(geometries[3], passThrough,  glm::translate(start) * glm::orientation(direction, glm::vec3(0.0f, 1.0f, 0.0f)) * scale);
	return ret;
}

Renderable* DebugShapes::addLine(glm::vec3 start, glm::vec3 stop, glm::vec3* color){
	float scale = glm::length(stop - start);
	glm::vec3 direction = glm::normalize(stop - start);
	Renderable* ret = renderer.addRenderable(geometries[3], shader, glm::translate(start) * glm::orientation(direction, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(glm::vec3(scale, scale, scale)));
	renderer.addUniformToRenderable(color, "uniformColor", VEC3, ret);
	return ret;
}

Renderable* DebugShapes::addCrosshairs(glm::vec3 location, glm::mat4 scale, ShaderInfo* passThrough){
	Renderable* ret = renderer.addRenderable(geometries[2], passThrough, glm::translate(location) * scale);
	return ret;
}


#endif