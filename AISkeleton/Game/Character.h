#pragma once
#include "glm.hpp"
#include "Rendering\Renderable.h"

class Character{
public:
	enum State{ SEEK, FOLLOW, FLEE };
	enum Type { ROCK, PAPER, SCISSORS };
	State currentState;
	Type type;
	Renderable* renderable;
	glm::vec3 pos;
	glm::vec3 velocity;
	glm::vec3 direction;
	bool isAlive;
};