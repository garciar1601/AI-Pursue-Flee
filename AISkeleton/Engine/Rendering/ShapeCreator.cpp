#include <Rendering\ShapeCreator.h>
#include <Rendering\MyVertex.h>

using glm::vec2;
using glm::vec3;
using glm::vec4;
MyVertex stackVerts[] = 
{
// Top
vec3(-1.0f, +1.0f, +1.0f), // 0
vec4(+1.0f, +0.0f, +0.0f, +1.0f), // Color
vec3(+0.0f, +1.0f, +0.0f), // Normal
vec2(+0.0f, +1.0f), // UV
vec3(+1.0f, +0.0f, +0.0f), //Tangent

vec3(+1.0f, +1.0f, +1.0f), // 1
vec4(+0.0f, +1.0f, +0.0f, +1.0f), // Color
vec3(+0.0f, +1.0f, +0.0f), // Normal
vec2(+1.0f, +1.0f), // UV
vec3(+1.0f, +0.0f, +0.0f), //Tangent

vec3(+1.0f, +1.0f, -1.0f), // 2
vec4(+0.0f, +0.0f, +1.0f, +1.0f), // Color
vec3(+0.0f, +1.0f, +0.0f), // Normal
vec2(+1.0f, +0.0f), // UV
vec3(+1.0f, +0.0f, +0.0f), //Tangent

vec3(-1.0f, +1.0f, -1.0f), // 3
vec4(+1.0f, +1.0f, +1.0f, +1.0f), // Color
vec3(+0.0f, +1.0f, +0.0f), // Normal
vec2(+0.0f, +0.0f), // UV
vec3(+1.0f, +0.0f, +0.0f), //Tangent

// Front
vec3(-1.0f, +1.0f, -1.0f), // 4
vec4(+1.0f, +0.0f, +1.0f, +1.0f), // Color
vec3(+0.0f, +0.0f, -1.0f), // Normal
vec2(+0.0f, +1.0f), // UV
vec3(+1.0f, +0.0f, +0.0f), //Tangent

vec3(+1.0f, +1.0f, -1.0f), // 5
vec4(+0.0f, +0.5f, +0.2f, +1.0f), // Color
vec3(+0.0f, +0.0f, -1.0f), // Normal
vec2(+1.0f, +1.0f), // UV
vec3(+1.0f, +0.0f, +0.0f), //Tangent

vec3(+1.0f, -1.0f, -1.0f), // 6
vec4(+0.8f, +0.6f, +0.4f, +1.0f), // Color
vec3(+0.0f, +0.0f, -1.0f), // Normal
vec2(+1.0f, +0.0f), // UV
vec3(+1.0f, +0.0f, +0.0f), //Tangent

vec3(-1.0f, -1.0f, -1.0f), // 7
vec4(+0.3f, +1.0f, +0.5f, +1.0f), // Color
vec3(+0.0f, +0.0f, -1.0f), // Normal
vec2(+0.0f, +0.0f), // UV
vec3(+1.0f, +0.0f, +0.0f), //Tangent

// Right
vec3(+1.0f, +1.0f, -1.0f), // 8
vec4(+0.2f, +0.5f, +0.2f, +1.0f), // Color
vec3(+1.0f, +0.0f, +0.0f), // Normal
vec2(+1.0f, +0.0f), // UV
vec3(+0.0f, +0.0f, -1.0f), //Tangent

vec3(+1.0f, +1.0f, +1.0f), // 9
vec4(+0.9f, +0.3f, +0.7f, +1.0f), // Color
vec3(+1.0f, +0.0f, +0.0f), // Normal
vec2(+0.0f, +0.0f), // UV
vec3(+0.0f, +0.0f, -1.0f), //Tangent

vec3(+1.0f, -1.0f, +1.0f), // 10
vec4(+0.3f, +0.7f, +0.5f, +1.0f), // Color
vec3(+1.0f, +0.0f, +0.0f), // Normal
vec2(+0.0f, +1.0f), // UV
vec3(+0.0f, +0.0f, -1.0f), //Tangent

vec3(+1.0f, -1.0f, -1.0f), // 11
vec4(+0.5f, +0.7f, +0.5f, +1.0f), // Color
vec3(+1.0f, +0.0f, +0.0f), // Normal
vec2(+1.0f, +1.0f), // UV
vec3(+0.0f, +0.0f, -1.0f), //Tangent

// Left
vec3(-1.0f, +1.0f, +1.0f), // 12
vec4(+0.7f, +0.8f, +0.2f, +1.0f), // Color
vec3(-1.0f, +0.0f, +0.0f), // Normal
vec2(+1.0f, +0.0f), // UV
vec3(+0.0f, +0.0f, +1.0f), //Tangent

vec3(-1.0f, +1.0f, -1.0f), // 13
vec4(+0.5f, +0.7f, +0.3f, +1.0f), // Color
vec3(-1.0f, +0.0f, +0.0f), // Normal
vec2(+0.0f, +0.0f), // UV
vec3(+0.0f, +0.0f, +1.0f), //Tangent

vec3(-1.0f, -1.0f, -1.0f), // 14
vec4(+0.4f, +0.7f, +0.7f, +1.0f), // Color
vec3(-1.0f, +0.0f, +0.0f), // Normal
vec2(+0.0f, +1.0f), // UV
vec3(+0.0f, +0.0f, +1.0f), //Tangent

vec3(-1.0f, -1.0f, +1.0f), // 15
vec4(+0.2f, +0.5f, +1.0f, +1.0f), // Color
vec3(-1.0f, +0.0f, +0.0f), // Normal
vec2(+1.0f, +1.0f), // UV
vec3(+0.0f, +0.0f, +1.0f), //Tangent

// Back
vec3(+1.0f, +1.0f, +1.0f), // 16
vec4(+0.6f, +1.0f, +0.7f, +1.0f), // Color
vec3(+0.0f, +0.0f, +1.0f), // Normal
vec2(+1.0f, +0.0f), // UV
vec3(+1.0f, +0.0f, +0.0f), //Tangent

vec3(-1.0f, +1.0f, +1.0f), // 17
vec4(+0.6f, +0.4f, +0.8f, +1.0f), // Color
vec3(+0.0f, +0.0f, +1.0f), // Normal
vec2(+0.0f, +0.0f), // UV
vec3(+1.0f, +0.0f, +0.0f), //Tangent

vec3(-1.0f, -1.0f, +1.0f), // 18
vec4(+0.2f, +0.8f, +0.7f, +1.0f), // Color
vec3(+0.0f, +0.0f, +1.0f), // Normal
vec2(+0.0f, +1.0f), // UV
vec3(+1.0f, +0.0f, +0.0f), //Tangent

vec3(+1.0f, -1.0f, +1.0f), // 19
vec4(+0.2f, +0.7f, +1.0f, +1.0f), // Color
vec3(+0.0f, +0.0f, +1.0f), // Normal
vec2(+1.0f, +1.0f), // UV
vec3(+1.0f, +0.0f, +0.0f), //Tangent

// Bottom
vec3(+1.0f, -1.0f, -1.0f), // 20
vec4(+0.8f, +0.3f, +0.7f, +1.0f), // Color
vec3(+0.0f, -1.0f, +0.0f), // Normal
vec2(+1.0f, +1.0f), // UV
vec3(+1.0f, +0.0f, +0.0f), //Tangent

vec3(-1.0f, -1.0f, -1.0f), // 21
vec4(+0.8f, +0.9f, +0.5f, +1.0f), // Color
vec3(+0.0f, -1.0f, +0.0f), // Normal
vec2(+0.0f, +1.0f), // UV
vec3(+1.0f, +0.0f, +0.0f), //Tangent

vec3(-1.0f, -1.0f, +1.0f), // 22
vec4(+0.5f, +0.8f, +0.5f, +1.0f), // Color
vec3(+0.0f, -1.0f, +0.0f), // Normal
vec2(+0.0f, +0.0f), // UV
vec3(+1.0f, +0.0f, +0.0f), //Tangent

vec3(+1.0f, -1.0f, +1.0f), // 23
vec4(+0.9f, +1.0f, +0.2f, +1.0f), // Color
vec3(+0.0f, -1.0f, +0.0f), // Normal
vec2(+1.0f, +0.0f), // UV
vec3(+1.0f, +0.0f, +0.0f), //Tangent
};
unsigned short stackIndices[] = {
0, 1, 2, 0, 2, 3, // Top
4, 5, 6, 4, 6, 7, // Front
8, 9, 10, 8, 10, 11, // Right 
12, 13, 14, 12, 14, 15, // Left
16, 17, 18, 16, 18, 19, // Back
20, 22, 21, 20, 23, 22, // Bottom
};

MyShapeData ShapeCreator::makeCube()
{

return copyToShapeData(stackVerts, sizeof(stackVerts)/sizeof(MyVertex), stackIndices, sizeof(stackIndices)/sizeof(stackIndices[0]));
}

MyShapeData ShapeCreator::copyToShapeData(MyVertex* verts, unsigned int numVerts, unsigned short* indices, unsigned int numIndices){
	MyShapeData data;
	data.verts = verts;
	data.numVerts = numVerts;
	data.indices = indices;
	data.numIndices = numIndices;
	return data;
}

