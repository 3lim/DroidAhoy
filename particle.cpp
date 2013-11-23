#include "headers/particle.h"

Particle::Particle() : 
	position(0,0),
	height(0),
	velocity(0,0),
	acceleration(0,0),
	massDensity(0.0),
	pressure(0.0),
	mass(0.02),
	radius(0.025)
{}

Particle::Particle(vec2 &position) : 
	position(position),
	height(0),
	velocity(0,0),
	acceleration(0,0),
	massDensity(0.0),
	pressure(0.0),
	mass(0.02),
	radius(0.025)
{}

Particle::Particle(float x, float y) : 
	position(x,y),
	height(0),
	velocity(0,0),
	acceleration(0,0),
	massDensity(0.0),
	pressure(0.0),
	mass(0.02),
	radius(0.025)
{}

Particle::~Particle() {}

void Particle::render() {
  glColor3f(0,0,1);
  glPointSize(5);
  glBegin(GL_POINTS);
  glVertex3f(position.x, position.y, height);
  glEnd();
}