#ifndef _SIMULATION_
#define _SIMULATION_

#include "particle.h"
#include "simulation_parameters.h"
#include "spatial_hashing.h"
#include "ocean_surface.h"
#include "boat.h"
#include <glm/glm.hpp>
#include <vector>

using std::vector;
using glm::mat4;

class SPHSimulation
{
public:
	SPHSimulation() {}
	SPHSimulation(const string&);
	~SPHSimulation();
	void update(float);
	void render();
	void draw(const mat4&);
	void addBoat(const Model& m) { boats.push_back(Boat(m)); }
private:
	SPHSimulation(const SimulationParameters&);
	int numberParticles;
	Particle* particles;

	OceanSurface oceanSurface;

	SpatialHashing spatialHashing;

	vector<Boat> boats;

	float sceneWidth, sceneLength;
	vec2 walls[4];
	vec2 wallsNormal[4];
	
	float kernelRadius;
	float massDensity0;
	float gravity;
	float rebound;
	float viscosityScale;
	float pressureScale;
	float heightOffset;
	float heightScale;

	void setupParticles();
	void computeMassDensityAndPressure();
	void computeForces();
	void integrateParticles(float);
};

#endif
