#pragma once
#include <Node.h>

class NodeSystem{
public:
	static const int MAX_NODES = 1000;
	int numNodes;	
	Node nodes[MAX_NODES];
};