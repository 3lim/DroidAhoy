#include "headers/ocean_surface.h"
#include <iostream>

using glm::dot;

OceanSurface::OceanSurface(float massDensity0, int nbRows, int nbColumns, float sceneWidth, float sceneLength, float heightOffset, float heightScale) :
	first(true),
	ib(0),
	massDensity0(massDensity0),
	nbRows(nbRows),
	nbColumns(nbColumns),
	sceneWidth(sceneWidth), 
	sceneLength(sceneLength),
	heightOffset(heightOffset),
	heightScale(heightScale)
{
	int numberVertices = (nbRows*nbColumns);
	oceanVertices.resize(3*numberVertices);
	oceanNormals.resize(3*numberVertices);
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
	oceanIndices.push_back(numberVertices+0);
	oceanIndices.push_back(numberVertices+1);
	oceanIndices.push_back(numberVertices+2);
	oceanIndices.push_back(numberVertices+0);
	oceanIndices.push_back(numberVertices+10);
	oceanIndices.push_back(numberVertices+2);

	oceanIndices.push_back(numberVertices+3);
	oceanIndices.push_back(numberVertices+4);
	oceanIndices.push_back(numberVertices+5);
	oceanIndices.push_back(numberVertices+3);
	oceanIndices.push_back(numberVertices+1);
	oceanIndices.push_back(numberVertices+5);

	oceanIndices.push_back(numberVertices+6);
	oceanIndices.push_back(numberVertices+7);
	oceanIndices.push_back(numberVertices+8);
	oceanIndices.push_back(numberVertices+6);
	oceanIndices.push_back(numberVertices+4);
	oceanIndices.push_back(numberVertices+8);

	oceanIndices.push_back(numberVertices+9);
	oceanIndices.push_back(numberVertices+10);
	oceanIndices.push_back(numberVertices+11);
	oceanIndices.push_back(numberVertices+9);
	oceanIndices.push_back(numberVertices+7);
	oceanIndices.push_back(numberVertices+11);

	float defaultHeight = 0.55+heightScale*heightOffset;
	for (int i = 0; i < nbRows; i++){
		for (int j = 0; j < nbColumns; j++){
			oceanVertices[3*(i*nbColumns+j)+0]=((float)i/(nbRows-1))*sceneLength-sceneLength/2;
			oceanVertices[3*(i*nbColumns+j)+1]=((float)j/(nbColumns-1))*sceneWidth-sceneWidth/2;
			oceanVertices[3*(i*nbColumns+j)+2]=defaultHeight;
		}
	}
	oceanVertices.push_back(-3.0f);
	oceanVertices.push_back(3.0f);
	oceanVertices.push_back(defaultHeight);

	oceanVertices.push_back(-3.0f);
	oceanVertices.push_back(-sceneLength/2);
	oceanVertices.push_back(defaultHeight);

	oceanVertices.push_back(-sceneWidth/2);
	oceanVertices.push_back(-sceneLength/2);
	oceanVertices.push_back(defaultHeight);

	oceanVertices.push_back(-3.0f);
	oceanVertices.push_back(-3.0f);
	oceanVertices.push_back(defaultHeight);
	
	oceanVertices.push_back(sceneWidth/2);
	oceanVertices.push_back(-3.0f);
	oceanVertices.push_back(defaultHeight);

	oceanVertices.push_back(sceneWidth/2);
	oceanVertices.push_back(-sceneLength/2);
	oceanVertices.push_back(defaultHeight);

	oceanVertices.push_back(3.0f);
	oceanVertices.push_back(-3.0f);
	oceanVertices.push_back(defaultHeight);

	oceanVertices.push_back(3.0f);
	oceanVertices.push_back(sceneLength/2);
	oceanVertices.push_back(defaultHeight);

	oceanVertices.push_back(sceneWidth/2);
	oceanVertices.push_back(sceneLength/2);
	oceanVertices.push_back(defaultHeight);

	oceanVertices.push_back(3.0f);
	oceanVertices.push_back(3.0f);
	oceanVertices.push_back(defaultHeight);

	oceanVertices.push_back(-sceneWidth/2);
	oceanVertices.push_back(3.0f);
	oceanVertices.push_back(defaultHeight);

	oceanVertices.push_back(-sceneWidth/2);
	oceanVertices.push_back(sceneLength/2);
	oceanVertices.push_back(defaultHeight);

	for (int i = 0; i < 12; i++){
		oceanNormals.push_back(0.0f);
		oceanNormals.push_back(0.0f);
		oceanNormals.push_back(1.0f);
	}


	glGenBuffers(1, &ib);
    glBindBuffer(GL_ARRAY_BUFFER, ib);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*oceanIndices.size(), oceanIndices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &vb);
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*oceanVertices.size(), oceanVertices.data(), GL_DYNAMIC_DRAW);

    glGenBuffers(1, &nb);
    glBindBuffer(GL_ARRAY_BUFFER, nb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*oceanNormals.size(), oceanNormals.data(), GL_DYNAMIC_DRAW);

    program_id = ShaderManager::get_program("basic");
    mvp_id = glGetUniformLocation(program_id, "MVP");
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

void OceanSurface::update(SpatialHashing& hashing, float kernelRadius){
	const array<int,9>& neighbourIndices = hashing.getAllNeighbouringCellIndices();
	HeightComputer computeHeight(kernelRadius);
	for (int i = 1; i < nbRows-1; i++){
		for (int j = 1; j < nbColumns-1; j++){
			int index = 3*(i*nbColumns+j);
			vec2 fixedPartPos(oceanVertices[index], oceanVertices[index+1]);
			float t1 = 0, t2 = 0;
			int cellIndex = hashing.getIndexBox(fixedPartPos);
			for (int k = 0; k < 9; k++){
				int neighbourIndex = cellIndex + neighbourIndices[k];
				if (hashing.areCellsNeighbours(cellIndex, neighbourIndex)){
					forward_list<Particle*>& particlesCell = hashing.getListParticles(neighbourIndex);
					for (Particle* p : particlesCell){
						computeHeight(p, fixedPartPos, t1, t2);
					}
				}
			}
			if (t2 > 0){
				oceanVertices[index+2] = heightScale*(t1/(t2*massDensity0)+heightOffset);
			}
		}
	}

	float xd = sceneWidth / nbRows;
	float yd = sceneLength / nbColumns;
	float z1,z2,z3,z4,xres,yres,zres=xd*yd;
	for (int i = 0; i < nbRows*nbColumns; i++){
		oceanNormals[3*i+0] = 0;
		oceanNormals[3*i+1] = 0;
		oceanNormals[3*i+2] = 0;
	} 
	for (int i = 0; i < nbRows-1; i++){
		for (int j = 0; j < nbColumns-1; j++){
			z1=oceanVertices[3*(i*nbColumns+j)+2];
			z2=oceanVertices[3*((i+1)*nbColumns+j)+2];
			z3=oceanVertices[3*(i*nbColumns+j+1)+2];
			z4=oceanVertices[3*((i+1)*nbColumns+j+1)+2];

			xres = yd*(z2-z1);
			yres = xd*(z4-z2);
			oceanNormals[3*(i*nbColumns+j)+0] += xres;
			oceanNormals[3*(i*nbColumns+j)+1] += yres;
			oceanNormals[3*(i*nbColumns+j)+2] += zres;
			oceanNormals[3*((i+1)*nbColumns+j)+0] += xres;
			oceanNormals[3*((i+1)*nbColumns+j)+1] += yres;
			oceanNormals[3*((i+1)*nbColumns+j)+2] += zres;
			oceanNormals[3*((i+1)*nbColumns+j+1)+0] += xres;
			oceanNormals[3*((i+1)*nbColumns+j+1)+1] += yres;
			oceanNormals[3*((i+1)*nbColumns+j+1)+2] += zres;

			xres = yd*(z4-z3); 
			yres = xd*(z3-z1);
			oceanNormals[3*(i*nbColumns+j)+0] += xres;
			oceanNormals[3*(i*nbColumns+j)+1] += yres;
			oceanNormals[3*(i*nbColumns+j)+2] += zres;
			oceanNormals[3*((i+1)*nbColumns+j)+0] += xres;
			oceanNormals[3*((i+1)*nbColumns+j)+1] += yres;
			oceanNormals[3*((i+1)*nbColumns+j)+2] += zres;
			oceanNormals[3*((i+1)*nbColumns+j+1)+0] += xres;
			oceanNormals[3*((i+1)*nbColumns+j+1)+1] += yres;
			oceanNormals[3*((i+1)*nbColumns+j+1)+2] += zres;
		}
	}

	for (int i = 0; i < nbRows*nbColumns; i++){
		float norm = sqrt(oceanNormals[3*i+0]*oceanNormals[3*i+0]+oceanNormals[3*i+1]*oceanNormals[3*i+1]+oceanNormals[3*i+2]*oceanNormals[3*i+2]);
		oceanNormals[3*i+0] /= norm;
		oceanNormals[3*i+1] /= norm;
		oceanNormals[3*i+2] /= norm;
	}
	{	
		glBindBuffer(GL_ARRAY_BUFFER, vb);
		float * bufferCopy = (float*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		for (int i = 0; i < nbRows*nbColumns*3; i++){
			bufferCopy[i] = oceanVertices[i];
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}
	{
		glBindBuffer(GL_ARRAY_BUFFER, nb);
		float * bufferCopy = (float*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		for (int i = 0; i < nbRows*nbColumns*3; i++){
			bufferCopy[i] = oceanNormals[i];
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}
}

void OceanSurface::render(){
	int numberFaces = 2*(nbRows-1)*(nbColumns-1);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glColor3f(0.0f,0.0f,1.0f);
	glPushMatrix();
	
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 3*(numberFaces+8); i++){
		glNormal3f(
			oceanNormals[3*oceanIndices[i]],
			oceanNormals[3*oceanIndices[i]+1],
			oceanNormals[3*oceanIndices[i]+2]
		);
		glVertex3f(
			oceanVertices[3*oceanIndices[i]],
			oceanVertices[3*oceanIndices[i]+1],
			oceanVertices[3*oceanIndices[i]+2]
		);
	}
	glEnd();
	
	glPopMatrix();
	
}

void OceanSurface::draw(const mat4 &vp){

	glUseProgram(program_id);
	
    glUniformMatrix4fv(mvp_id, 1, GL_FALSE, &vp[0][0]);
    
    //Vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    //normals
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, nb);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
    glDrawElements(
        GL_TRIANGLES,             
        oceanIndices.size(),    
        GL_UNSIGNED_INT,          
        (void*)0
    );

    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(0);
}

float OceanSurface::interpolateHeightAtPosition(const vec2 &pos){
	const vec2 bottomLeftCorner(-sceneWidth/2, -sceneLength/2);
	const vec2 tmp(pos-bottomLeftCorner);
	float x = tmp.x * nbColumns / sceneWidth;
	float y = tmp.y * nbRows / sceneLength;
	int xmap = floor(x), ymap = floor(y);
	if (xmap > nbColumns-1){
		xmap = nbColumns-1;
	} else if (xmap < 0) {
		xmap = 0;
	}
	if (ymap > nbRows-1){
		ymap = nbRows-1;
	} else if (ymap < 0) {
		ymap = 0;
	}
	if ((xmap == x || x > nbColumns-1 || x < 0) && (ymap == y || y > nbRows-1 || y < 0)){	
		return oceanVertices[3*(ymap*nbColumns+xmap)+2];
	}
	if (xmap == x || x > nbColumns-1 || x < 0){
		return oceanVertices[3*(ymap*nbColumns+xmap)+2]*(xmap+1-x)*(ymap+1-y)+oceanVertices[3*((ymap+1)*nbColumns+xmap)+2]*(xmap+1-x)*(y-ymap);
	}
	if (ymap == y || y > nbRows-1 || y < 0){
		return oceanVertices[3*(ymap*nbColumns+xmap)+2]*(xmap+1-x)*(ymap+1-y)+oceanVertices[3*(ymap*nbColumns+xmap+1)+2]*(x-xmap)*(ymap+1-y);
	}
	return oceanVertices[3*(ymap*nbColumns+xmap)+2]*(xmap+1-x)*(ymap+1-y)+oceanVertices[3*(ymap*nbColumns+xmap+1)+2]*(x-xmap)*(ymap+1-y)+oceanVertices[3*((ymap+1)*nbColumns+xmap)+2]*(xmap+1-x)*(y-ymap)+oceanVertices[3*((ymap+1)*nbColumns+xmap+1)+2]*(x-xmap)*(y-ymap);	
}

float OceanSurface::interpolateHeightAtPosition(const vec3 &pos){
	return interpolateHeightAtPosition(vec2(pos.x, pos.y));
}

vec3 OceanSurface::interpolateNormalAtPosition(const vec2 &pos){
	const vec2 bottomLeftCorner(-sceneWidth/2, -sceneLength/2);
	const vec2 tmp(pos-bottomLeftCorner);
	float x = tmp.x * nbColumns / sceneWidth;
	float y = tmp.y * nbRows / sceneLength;
	int xmap = floor(x), ymap = floor(y);
	if (xmap > nbColumns-1){
		xmap = nbColumns-1;
	} else if (xmap < 0) {
		xmap = 0;
	}
	if (ymap > nbRows-1){
		ymap = nbRows-1;
	} else if (ymap < 0) {
		ymap = 0;
	}
	if ((xmap == x || x > nbColumns-1 || x < 0) && (ymap == y || y > nbRows-1 || y < 0)){	
		return vec3(oceanNormals[3*(ymap*nbColumns+xmap)], oceanNormals[3*(ymap*nbColumns+xmap)+1], oceanNormals[3*(ymap*nbColumns+xmap)+2]);
	}
	if (xmap == x || x > nbColumns-1 || x < 0){
		return vec3(oceanNormals[3*(ymap*nbColumns+xmap)]*(xmap+1-x)*(ymap+1-y)+oceanNormals[3*((ymap+1)*nbColumns+xmap)]*(xmap+1-x)*(y-ymap),
					oceanNormals[3*(ymap*nbColumns+xmap)+1]*(xmap+1-x)*(ymap+1-y)+oceanNormals[3*((ymap+1)*nbColumns+xmap)+1]*(xmap+1-x)*(y-ymap),
					oceanNormals[3*(ymap*nbColumns+xmap)+2]*(xmap+1-x)*(ymap+1-y)+oceanNormals[3*((ymap+1)*nbColumns+xmap)+2]*(xmap+1-x)*(y-ymap));
	}
	if (ymap == y || y > nbRows-1 || y < 0){
		return vec3(oceanNormals[3*(ymap*nbColumns+xmap)]*(xmap+1-x)*(ymap+1-y)+oceanNormals[3*(ymap*nbColumns+xmap+1)]*(x-xmap)*(ymap+1-y),
					oceanNormals[3*(ymap*nbColumns+xmap)+1]*(xmap+1-x)*(ymap+1-y)+oceanNormals[3*(ymap*nbColumns+xmap+1)+1]*(x-xmap)*(ymap+1-y),
					oceanNormals[3*(ymap*nbColumns+xmap)+2]*(xmap+1-x)*(ymap+1-y)+oceanNormals[3*(ymap*nbColumns+xmap+1)+2]*(x-xmap)*(ymap+1-y));
	}
	return vec3(oceanNormals[3*(ymap*nbColumns+xmap)]*(xmap+1-x)*(ymap+1-y)+oceanNormals[3*(ymap*nbColumns+xmap+1)]*(x-xmap)*(ymap+1-y)+oceanNormals[3*((ymap+1)*nbColumns+xmap)]*(xmap+1-x)*(y-ymap)+oceanNormals[3*((ymap+1)*nbColumns+xmap+1)]*(x-xmap)*(y-ymap),
			oceanNormals[3*(ymap*nbColumns+xmap)+1]*(xmap+1-x)*(ymap+1-y)+oceanNormals[3*(ymap*nbColumns+xmap+1)+1]*(x-xmap)*(ymap+1-y)+oceanNormals[3*((ymap+1)*nbColumns+xmap)+1]*(xmap+1-x)*(y-ymap)+oceanNormals[3*((ymap+1)*nbColumns+xmap+1)+1]*(x-xmap)*(y-ymap),
			oceanNormals[3*(ymap*nbColumns+xmap)+2]*(xmap+1-x)*(ymap+1-y)+oceanNormals[3*(ymap*nbColumns+xmap+1)+2]*(x-xmap)*(ymap+1-y)+oceanNormals[3*((ymap+1)*nbColumns+xmap)+2]*(xmap+1-x)*(y-ymap)+oceanNormals[3*((ymap+1)*nbColumns+xmap+1)+2]*(x-xmap)*(y-ymap));	
}

vec3 OceanSurface::interpolateNormalAtPosition(const vec3 &pos){
	return interpolateNormalAtPosition(vec2(pos.x, pos.y));
}