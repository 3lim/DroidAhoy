#ifndef _SIMULATION_
#define _SIMULATION_

#include "particle.h"
#include "simulation_parameters.h"
#include <vector>

using std::vector;

class Simulation
{
public:
	Simulation(int, const string& = "");
	void update(float);
	void render();
private:
	int numberParticles;
	// vector<Particle*> particles;
	Particle* particles;
	vector<vec3> accelerationAtParticles;// x: number density
										 // y: near number density
										 // z: massdensity
	vector<vec3> densityAtParticles;

	SimulationParameters parameters;

	// float influenceRadiusScale;
	// float hardCoreRadius;
	// float targetNumberDensity;
	// float ambientDensity;
	// float nearToFar;
	// float stiffness;
	// float viscousGain;
	// float radialViscosity;
	// vec3 gravityAcceleration;
	
	void computeDensityAtParticles();
	void computePressureGradientForce();
	void applyExternalForces();
	void diffuseAndDissipateVelocity(float);
};

#endif
