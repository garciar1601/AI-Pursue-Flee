#include "AStar.h"
#include "AStarNode.h"
#include <glm.hpp>

void AStar::aStar(int start, int end, NodeSystem nodes)
{
	AStarNode* closedList[MAX_NODES];
	AStarNode* openList[MAX_NODES];

	int openPlace = 0;
	int closedPlace = 0;

	AStarNode* startNode = new AStarNode();
	startNode->id = start;
	startNode->cost = 0;
	startNode->TEC = startNode->cost + glm::length(nodes.nodes[startNode->id].position - nodes.nodes[end].position);
	startNode->parent = NULL;

	openList[openPlace] = startNode;
	openPlace++;

	bool pathFound = false;
	while(!pathFound){
		AStarNode* current = NULL;
		for(int i = 0; i < openPlace; i++){
			if(current == NULL){
				current = openList[i];
			}
			else{
				if(openList[i]->TEC < current->TEC){
					current = openList[i];
				}
			}
		}
		for(int i = 0; i < openPlace; i++){
			if(openList[i] != NULL){
				if(current->id == openList[i]->id){
					openList[i] = NULL;
					openPlace--;
				}
			}
			if(openList[i] == NULL){
				openList[i] = openList[i+1];
				openList[i+1] = NULL;
			}
		}
		closedList[closedPlace] = current;
		closedPlace++;
		if(current->id == end){
			pathFound = true;
		}
		Node currentNode = nodes.nodes[current->id];
		for(int i = 0; i < currentNode.numConnections; i++){
			Node neighborNode = nodes.nodes[currentNode.connections[i]];
			AStarNode* neighbor = new AStarNode();
			neighbor->id = neighborNode.id;
			neighbor->cost = current->cost + glm::length(currentNode.position - neighborNode.position);
			neighbor->TEC = neighbor->cost + glm::length(neighborNode.position - nodes.nodes[end].position);
			neighbor->parent = current;
			bool addToList = true;
			for(int i2 = 0; i2 < openPlace; i2++){
				if(openList[i2] != NULL){
					if(neighbor->id == openList[i2]->id){
						if(neighbor->cost < openList[i2]->cost){
							delete openList[i2];
							openList[i2] = NULL;
							openPlace--;
						}
						else{
							addToList = false;
						}
					}
				}
				if(openList[i2] == NULL){
					openList[i2] = openList[i2+1];
					openList[i2+1] = NULL;
				}
			}
			for(int i2 = 0; i2 < closedPlace; i2++){
				if(closedList[i2] != NULL){
					if(neighbor->id == closedList[i2]->id){
						if(neighbor->cost < closedList[i2]->cost){
							delete closedList[i2];
							closedList[i2] = NULL;
							closedPlace--;
						}
						else{
							addToList = false;
						}
					}
				}
				if(closedList[i2] == NULL){
					closedList[i2] = closedList[i2+1];
					closedList[i2+1] = NULL;
				}
			}
			if(addToList){
				openList[openPlace] = neighbor;
				openPlace++;
			}
		}
	}
	AStarNode* current = closedList[closedPlace-1];
	int reversePath[MAX_NODES];
	int num = 0;
	bool endOfPath = false;
	while(!endOfPath){
		reversePath[num] = current->id;
		num++;
		if(current->parent != NULL){
			AStarNode n = *current;
			current = n.parent;
		}
		else{
			endOfPath = true;
		}
	}
	int nodesInPath = num;
	num--;	
	for(int i = 0; i < nodesInPath;i ++){
		pathList[i] = reversePath[num];
		num --;
	}
	currentNode = 0;
	for(int i = 0; i < openPlace; i++){
		delete openList[i];
	}

	for(int i = 0; i < closedPlace; i++){
		delete closedList[i];
	}
}
