#ifndef _SIMULATION_
#define _SIMULATION_

#include "particle.h"
#include "simulation_parameters.h"
#include "wall.h"
#include <vector>

using std::vector;

class SPHSimulation
{
public:
	SPHSimulation(int, const string& = "");
	void update(float);
	void render();
  void update_vertices();
  vector<float> particle_vertices;
	const int numberParticles;

private:
	Particle* particles;
	vector<vec3> accelerationAtParticles;// x: number density
										 // y: near number density
										 // z: massdensity
	vector<vec3> densityAtParticles;

	int numberWalls;
	Wall* walls;

	SimulationParameters parameters;
	
	void computeDensityAtParticles();
	void computePressureGradientForce();
	void applyExternalForces();
	void diffuseAndDissipateVelocity(float);
};

#endif
