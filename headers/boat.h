#ifndef _BOAT_
#define _BOAT_

#include "particle.h"
#include "model.h"

class Boat : public Particle{
public:
	Boat();
	void update();
private:
	Model model;
};

#endif