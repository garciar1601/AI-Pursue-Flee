#ifndef PARTICLE_H
#define PARTICLE_H

#include "Core.h"
#include "Vector2.h"
#include "Color.h"

using Core::RGB;
using Engine::Vector2;

class Particle{
public:
	Vector2 position;
	Vector2 velocity;
	float lifetime;
	Color color;
	Particle(){};
	Particle(Vector2 pos, Vector2 vlcty, float life, Color colr){position = pos; velocity = vlcty; lifetime = life; color = colr;};
};

#endif