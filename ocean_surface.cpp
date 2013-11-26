#include "headers/ocean_surface.h"
#include <GL/glu.h>	
#include <iostream>

using glm::dot;

OceanSurface::OceanSurface(float massDensity0, int nbRows, int nbColumns, float sceneWidth, float sceneLength, float heightOffset, float heightScale) :
	massDensity0(massDensity0),
	nbRows(nbRows),
	nbColumns(nbColumns),
	sceneWidth(sceneWidth), 
	sceneLength(sceneLength),
	heightOffset(heightOffset),
	heightScale(heightScale)
{
	oceanVertices.resize(3*nbRows*nbColumns);
	oceanIndices.clear();
	for (int i = 0; i < nbRows-1; i++){
		for (int j = 0; j < nbColumns-1; j++){
			oceanIndices.push_back(j+i*nbColumns);
			oceanIndices.push_back(j+1+i*nbColumns);
			oceanIndices.push_back(j+1+(i+1)*nbColumns);
			oceanIndices.push_back(j+i*nbColumns);
			oceanIndices.push_back(j+(i+1)*nbColumns);
			oceanIndices.push_back(j+1+(i+1)*nbColumns);
		}
	}

	for (int i = 0; i < nbRows; i++){
		for (int j = 0; j < nbColumns; j++){
			oceanVertices[3*(i*nbColumns+j)+0]=((float)i/(nbRows-1))*sceneLength-sceneLength/2;
			oceanVertices[3*(i*nbColumns+j)+1]=((float)j/(nbColumns-1))*sceneWidth-sceneWidth/2;
			oceanVertices[3*(i*nbColumns+j)+2]=0.0f;
		}
	}
}


class HeightComputer{
public:
  HeightComputer(float kernelRadius) :
    kernelRadius2(pow(kernelRadius, 2)),
    cst(4/(Pi*pow(kernelRadius,8)))
  {}
  void operator()(Particle* p, const vec2& position, float& t1, float& t2){
    const vec2 r = position - p->position;
    const float norm2 = dot(r,r);
    if (norm2 < kernelRadius2) {
      const float tmp = cst * pow(kernelRadius2-norm2,3);
      t1 += p->massDensity * tmp;
      t2 += tmp;
    }
  }
private:
  const float kernelRadius2;
  const float cst;
};

void OceanSurface::update(SpatialHashing& hashing, const float kernelRadius){
	const array<int,9>& neighbourIndices = hashing.getAllNeighbouringCellIndices();
	HeightComputer computeHeight(kernelRadius);
	for (int i = 0; i < nbRows*nbColumns; i++){
		vec2 fixedPartPos(oceanVertices[3*i], oceanVertices[3*i+1]);
		float t1 = 0, t2 = 0;
		int cellIndex = hashing.getIndexBox(fixedPartPos);
		// std::cout << cellIndex << std::endl;
		for (int j = 0; j < 9; j++){
			int neighbourIndex = cellIndex + neighbourIndices[j];
			if (hashing.areCellsNeighbours(cellIndex, neighbourIndex)){
				forward_list<Particle*>& particlesCell = hashing.getListParticles(neighbourIndex);
				for (Particle* p : particlesCell){
					computeHeight(p, fixedPartPos, t1, t2);
				}
			}
		}
		oceanVertices[3*i+2] = heightScale*(t1/(t2*massDensity0)+heightOffset);
		// std::cout << " " << oceanVertices[3*i+2] << std::endl;
	}
}

void OceanSurface::render(){
	int numberFaces = 2*(nbRows-1)*(nbColumns-1);
	glColor3f(1.0f,1.0f,0.0f);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 3*numberFaces; i++){
		glVertex3f(
			oceanVertices[3*oceanIndices[i]],
			oceanVertices[3*oceanIndices[i]+1],
			oceanVertices[3*oceanIndices[i]+2]
		);
	}
	glEnd();
}