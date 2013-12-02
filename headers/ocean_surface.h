#ifndef _OCEANSURFACE_
#define _OCEANSURFACE_

#include "libs.h"
#include "shader_manager.h"
#include "spatial_hashing.h"
#include "particle.h"
//#include "boat.h"
#include <glm/glm.hpp>
#include <vector>

using glm::mat4;
using std::vector;

class OceanSurface{
public:
	OceanSurface() {}
	OceanSurface(float, int, int, float, float, float, float);
	const vector<float>& getOceanVertices() { return oceanVertices; }
	const vector<unsigned>& getOceanIndices() { return oceanIndices; }
	const vector<unsigned>& getOceanNormals() { return oceanIndices; }
	void update(SpatialHashing& hashing, float kernelRadius);
	void render();
	void draw(const mat4&);
	float interpolateHeightAtPosition(const vec2&);
	float interpolateHeightAtPosition(const vec3&);
	vec3 interpolateNormalAtPosition(const vec2&);
	vec3 interpolateNormalAtPosition(const vec3&);
private:
	bool first;
	//buffer ids
	GLuint ib, vb, nb;
	//Uniform ids, program id
    GLuint mvp_id, program_id;

    vector<unsigned> oceanIndices;
    vector<float> oceanVertices;
    vector<float> oceanNormals;

	float massDensity0;
	int nbRows, nbColumns;
	float sceneWidth, sceneLength;
	float heightOffset, heightScale;
};

#endif