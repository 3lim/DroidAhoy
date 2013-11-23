#ifndef _PARTICLE_
#define _PARTICLE_

#include <GL/glu.h>
#include <glm/glm.hpp>

using glm::vec2;

static const float Pi = 3.1415926535897932384626433832795f;

class Particle{
private:
	vec2 position;
	vec2 velocity;
	vec2 acceleration;
public:
	Particle();
	Particle(vec2&);
	Particle(float, float);
	~Particle();
	void render();
	const vec2& getPosition() const { return position; }
	const vec2& getVelocity() const { return velocity; }
	const vec2& getAcceleration() const  { return acceleration; }
	void updatePosition(float timeStep) { position += velocity * timeStep; }
	void setPosition(const vec2& newPos) { position = newPos; }
	void incrPosition(const vec2& incr) { position += incr; }
	void updateVelocity(float timeStep) { velocity += acceleration * timeStep; }
	void setVelocity(const vec2& newVel) { position = newVel; }
	void incrVelocity(const vec2& incr) { velocity += incr; }
	void setAcceleration(const vec2& newAcc) { acceleration = newAcc; }
	void incrAcceleration(const vec2& incr) { acceleration += incr; }

	float height;
	float massDensity;
	float pressure;
	const float mass;
	const float radius;

};

#endif