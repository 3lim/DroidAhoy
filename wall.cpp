#include "headers/wall.h"
#include "glm/glm.hpp"

using glm::dot;
using glm::cross;
using glm::length;

Wall::Wall(vec3 corner1, vec3 corner2, vec3 corner3, vec3 corner4) :
	corner1(corner1),
	corner2(corner2),
	corner3(corner3),
	corner4(corner4)
{
	normal = cross(corner1-corner2, corner3-corner2);
}

Wall::~Wall() {}

void Wall::detectCollision(const vec3& oldPosition, Particle& pcl){
	const float r = dot(normal, corner1 - oldPosition) / dot(normal, pcl.getPosition() - oldPosition);
	if (r > 0.0 && r < 1.0)
	{
		vec3 collisionPoint(oldPosition + r * (pcl.getPosition() - oldPosition));
		// if ()
		// {
			pcl.getPosition() = collisionPoint;
		// }
	}
}