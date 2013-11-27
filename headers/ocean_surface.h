#ifndef _OCEANSURFACE_
#define _OCEANSURFACE_

#include "spatial_hashing.h"
#include "particle.h"
#include <vector>

using std::vector;

class OceanSurface{
public:
	OceanSurface(float, int, int, float, float, float, float);
	const vector<float>& getOceanVertices() { return oceanVertices; }
	const vector<unsigned>& getOceanIndices() { return oceanIndices; }
	void update(SpatialHashing& hashing, float kernelRadius);
	void render();
private:
	vector<float> oceanVertices;
	vector<float> oceanNormals;
	vector<unsigned> oceanIndices;
	float massDensity0;
	int nbRows, nbColumns;
	float sceneWidth, sceneLength;
	float heightOffset, heightScale;
};

#endif