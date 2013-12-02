#ifndef _BOAT_
#define _BOAT_

#include "particle.h"
#include "model.h"
#include <iostream>

class Boat : public Particle{
public:
	Boat(const Model&);
	//void setDirection();
	void update();
	void draw(const mat4&);
	void check() { std::cout << model.get_position().z << std::endl; }
private:
	Model model;
};

#endif