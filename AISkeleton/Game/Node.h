#pragma once
#include <glm.hpp>

class Node{
public:
	int id;
	glm::vec3 position;
	int numConnections;
	static const int MAX_CONNECTIONS = 50;
	int connections[MAX_CONNECTIONS];
	Node() : id(-1){}
};