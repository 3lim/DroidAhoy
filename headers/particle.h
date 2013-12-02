#ifndef _PARTICLE_
#define _PARTICLE_

#include "libs.h"
#include <glm/glm.hpp>

using glm::vec2;
using glm::vec3;
using glm::dot;

static const float Pi = 3.1415926535897932384626433832795f;

class Particle{
private:
public:
	Particle();
	Particle(vec2&);
	Particle(float, float);
	~Particle();
	void render();
	void updateWalls(vec2 (&walls)[4], vec2 (&wallsNormal)[4], float);
	void updatePosition(float timeStep) { position += velocity * timeStep; }
	void updateVelocity(float timeStep) { velocity += acceleration * timeStep; }

	vec2 position;
	vec2 velocity;
	vec2 acceleration;
	float height;
	float massDensity;
	float pressure;
	const float mass;
	const float radius;

};

#endif