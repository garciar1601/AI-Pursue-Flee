#pragma once
class AStarNode{
public:
	AStarNode* parent;
	int id;
	float cost;
	float TEC;
};