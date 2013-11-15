#ifndef _SIMULATION_
#define _SIMULATION_

#include "particle.h"
#include <vector>

using std::vector;

class Simulation
{
public:
	Simulation(int);
	void update(float);
	void render();
private:
	int numberParticles;
	float influenceRadiusScale;
	float hardCoreRadius;
	float targetNumberDensity;
	float stiffness;
	float nearToFar;
	float viscousGain;
	float ambientDensity;
	float radialViscosity;
	vec3 gravityAcceleration;
	vector<Particle*> particles;
	vector<vec3> accelerationAtParticles;
	vector<vec3> densityAtParticles;
	void computeDensityAtParticles();
	void computePressureGradientForce();
	void applyExternalForces();
	void diffuseAndDissipateVelocity(float);
};

#endif
