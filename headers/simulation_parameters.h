#ifndef _SIMULATION_PARAMETERS_
#define _SIMULATION_PARAMETERS_

#include <string>
#include "glm/glm.hpp"

using std::string;
using glm::vec3;

class SimulationParameters
{
public:
	SimulationParameters(const string&);
	string getInitialSetup() const { return initialSetup; }
	int getNumberParticles() const { return numberParticles; }
	float getSceneLength() const { return sceneLength; }
	float getSceneWidth() const { return sceneWidth; }
	float getKernelRadius() const { return kernelRadius; }
	float getMassDensity0() const { return massDensity0; }
	float getGravity() const { return gravity; }
	float getRebound() const { return rebound; }
	float getViscosityScale() const { return viscosityScale; }
	float getPressureScale() const { return pressureScale; }
	float getHeightOffset() const { return heightOffset; }
	float getHeightScale() const { return heightScale; }
private:
	string initialSetup;
	int numberParticles;
	float sceneWidth;
	float sceneLength;
	float kernelRadius;
	float massDensity0;
	float gravity;
	float rebound;
	float viscosityScale;
	float pressureScale;
	float heightOffset;
	float heightScale;
};

#endif