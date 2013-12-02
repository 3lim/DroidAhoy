#include "headers/particle.h"

Particle::Particle() : 
	position(0,0),
	velocity(0,0),
	acceleration(0,0),
	height(0),
	massDensity(0.0),
	pressure(0.0),
	mass(0.02),
	radius(0.025)
{}

Particle::Particle(vec2 &position) : 
	position(position),
	velocity(0,0),
	acceleration(0,0),
	height(0),
	massDensity(0.0),
	pressure(0.0),
	mass(0.02),
	radius(0.025)
{}

Particle::Particle(float x, float y) : 
	position(x,y),
	velocity(0,0),
	acceleration(0,0),
	height(0),
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

void Particle::updateWalls(vec2 (&walls)[4], vec2 (&wallsNormal)[4], float rebound){
	for (int j = 0; j < 4; j++){
      float penetration = dot((position - walls[j]), wallsNormal[j]);
      if (penetration < 0) {
        float vPen = dot(velocity, wallsNormal[j]);
        position += -penetration * wallsNormal[j];
        velocity += -(1 + rebound) * vPen * wallsNormal[j];
      }
    }
}