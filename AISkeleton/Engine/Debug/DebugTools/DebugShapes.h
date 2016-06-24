#ifndef DEBUG_SHAPES_H
#define DEBUG_SHAPES_H
#include <Rendering\Renderable.h>
#include <glm.hpp>
#include <ExportHeader.h>
class GeometryInfo;
struct ShaderInfo;
class DebugShapes
{
	static const unsigned int MAX_GEOMETRIES = 10;
	GeometryInfo* geometries[MAX_GEOMETRIES];
	static DebugShapes* instance;
	DebugShapes() {}
	DebugShapes(const DebugShapes&);
	DebugShapes& operator=(const DebugShapes&);
	void addGeometries();
	ShaderInfo* shader;
	unsigned int numGeometries;
public:
#ifdef DEBUG_SHAPES
	ENGINE_SHARED static DebugShapes& getInstance() { return *instance; }
	ENGINE_SHARED bool initialize();
	ENGINE_SHARED Renderable* addSphere(glm::vec3 location, glm::mat4 rotation, glm::mat4 scale, glm::vec3* color);
	ENGINE_SHARED Renderable* addCube(glm::vec3 location, glm::mat4 rotation, glm::mat4 scale, glm::vec3* color);
	ENGINE_SHARED Renderable* addVector(glm::vec3 start, glm::vec3 facing, glm::mat4 scale, ShaderInfo* passThrough);
	ENGINE_SHARED Renderable* addLine(glm::vec3 start, glm::vec3 stop, glm::vec3* color);
	ENGINE_SHARED Renderable* addCrosshairs(glm::vec3 location, glm::mat4 scale, ShaderInfo* passThrough);
	ENGINE_SHARED void update();
	ENGINE_SHARED void sendShader(ShaderInfo* shader);
#else
	ENGINE_SHARED static DebugShapes& getInstance() { return *instance; }
	ENGINE_SHARED bool initialize(){ return false; }
	ENGINE_SHARED Renderable* addSphere(glm::vec3, glm::mat4, glm::mat4, glm::vec3*){ return 0; }
	ENGINE_SHARED Renderable* addCube(glm::vec3, glm::mat4, glm::mat4, glm::vec3*){ return 0; }
	ENGINE_SHARED Renderable* addVector(glm::vec3, glm::vec3, glm::mat4, ShaderInfo*){ return 0; }
	ENGINE_SHARED Renderable* addLine(glm::vec3, glm::vec3, glm::vec3*){ return 0; }
	ENGINE_SHARED Renderable* addCrosshairs(glm::vec3, glm::mat4, ShaderInfo*){ return 0; }
	ENGINE_SHARED void update(){}
	ENGINE_SHARED void sendShader(ShaderInfo*){}
#endif
};

#define debugShapes DebugShapes::getInstance()

#endif