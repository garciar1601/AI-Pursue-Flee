#pragma once
#include <Node.h>
#include <NodeSystem.h>
class AStar
{
public:
	static const int MAX_NODES = 100;
	int pathList[MAX_NODES];
	int currentNode;
	void aStar(int start, int end, NodeSystem nodes);
};

