#ifndef _BOAT_
#define _BOAT_

#include "particle.h"
#include "model.h"
#include <iostream>

class Boat : public Particle{
public:
	Boat();
	Boat(const Model&);
	//void setDirection();
	void update();
	void draw(const mat4&);
	void render();
private:
	Model model;
};

#endif