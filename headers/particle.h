#ifndef _PARTICLE_
#define _PARTICLE_

#include <glm/glm.hpp>

using glm::vec3;

static const float Pi = 3.1415926535897932384626433832795f ;

class Particle{
public:
	Particle();
	Particle(vec3&);
	Particle(float, float, float);
	~Particle();
	void updateVelocity(float timeStep, vec3 acceleration) { velocity += acceleration * timeStep; }
	void updatePosition(float timeStep) { position += velocity * timeStep; }
	vec3& getPosition() { return position; }
	const vec3& getPosition() const { return position; }
	vec3& getVelocity() { return velocity; }
	const vec3& getVelocity() const { return velocity; }
	const float& getDensity() const { return density; }
	const float& getSize() const { return size; }
	const float getRadius() const { return size*0.5f; 	}
	const float getMass() const { return density * getVolume(); }
	const float getVolume() const { return Pi*size*size*size/6; }
	void incrPosition(vec3 &inc) { position += inc; }
	void incrVelocity(vec3 &inc) { velocity += inc; }

private:
	vec3 position;
	vec3 velocity;
	float density;
	float size;
};

#endif