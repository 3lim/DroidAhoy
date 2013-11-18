#ifndef _WALL_
#define _WALL_

#include <glm/glm.hpp>
#include "particle.h"

// a wall is a rectangle

class Wall 
{
public:
	Wall(vec3, vec3, vec3, vec3);
	~Wall();
	void detectCollision(const vec3&, Particle&);
private:
	vec3 corner1;
	vec3 corner2;
	vec3 corner3;
	vec3 corner4;
	vec3 normal;
};

#endif