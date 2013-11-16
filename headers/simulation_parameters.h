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
	const float& getInfluenceRadiusScale() { return influenceRadiusScale; }
	const float& getHardCoreRadius() { return hardCoreRadius; }
	const float& getTargetNumberDensity() { return targetNumberDensity; }
	const float& getAmbientDensity() { return ambientDensity; }
	const float& getNearToFar() { return nearToFar; }
	const float& getStiffness() { return stiffness; }
	const float& getViscousGain() { return viscousGain; }
	const float& getRadialViscosity() { return radialViscosity; }
	const vec3& getGravityAcceleration() { return gravityAcceleration; }
private:
	float influenceRadiusScale;
	float hardCoreRadius;
	float targetNumberDensity;
	float ambientDensity;
	float nearToFar;
	float stiffness;
	float viscousGain;
	float radialViscosity;
	vec3  gravityAcceleration;
};

#endif