#include "headers/particle.h"

Particle::Particle() : 
	position(0,0,0),
	velocity(0,0,0),
	density(0),
	size(0)
{}

Particle::Particle(vec3 &position) : 
	position(position),
	velocity(0,0,0),
	density(0),
	size(0)
{}

Particle::Particle(float x, float y, float z) : 
	position(x,y,z),
	velocity(0,0,0),
	density(0),
	size(0)
{}