#ifndef SHAPE_CREATOR_H
#define SHAPE_CREATOR_H
#include <Rendering\MyShapeData.h>
#include <ExportHeader.h>
class ShapeCreator
{
	static MyShapeData copyToShapeData(MyVertex* verts, unsigned int numVerts, unsigned short* indices, unsigned int numIndices);
public:
	ENGINE_SHARED static MyShapeData makeCube();
};

#endif