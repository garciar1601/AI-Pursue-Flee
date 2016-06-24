#ifndef MYSHAPE_DATA_H
#define MYSHAPE_DATA_H
#include <Rendering\MyVertex.h>

struct MyShapeData
{
	MyVertex* verts;
	unsigned int numVerts;
	unsigned short* indices;
	unsigned int numIndices;
	MyShapeData() : verts(0), numVerts(0), indices(0), numIndices(0) {}
	unsigned int vertexBufferSize() const { return numVerts * sizeof(MyVertex); }
	unsigned int indexBufferSize() const { return numIndices * sizeof(unsigned short); }
	void cleanUp()
	{
		delete[] verts;
		verts = 0;
		delete[] indices;
		indices = 0;
		numVerts = numIndices = 0;
	}
};

#endif