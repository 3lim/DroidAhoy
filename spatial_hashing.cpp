#include "headers/spatial_hashing.h"
// #include <algorithm>

SpatialHashing::SpatialHashing(float sizeSquare, float sceneWidth, float sceneLength) :
	sizeSquare(sizeSquare),
	bottomLeftCorner(-sceneWidth/2, -sceneLength/2),
	nbRows(ceil(sceneWidth/sizeSquare)),
	nbColumns(ceil(sceneLength/sizeSquare)),
	forwardNeighbouringCellIndices{
					  				 1 		  ,
		 nbColumns-1,  nbColumns, nbColumns+1
	},
	allNeighbouringCellIndices{
		-nbColumns-1, -nbColumns, -nbColumns+1,
				  -1, 		0,	   1 		  ,
		 nbColumns-1,  nbColumns, nbColumns+1	
	}
{}

void SpatialHashing::clear() {
	for (int i = 0; i < nbRows*nbColumns; i++){
		hashmap[i].clear();
	}
}

int SpatialHashing::getIndexBox(const vec2 &position){
	const vec2 tmp = position-bottomLeftCorner;
	return floor(tmp.y / sizeSquare)*nbColumns + floor(tmp.x / sizeSquare);
}

/*inline*/ int SpatialHashing::getNumberCells(){
	return nbRows*nbColumns;
}

/*inline*/ forward_list<Particle*>& SpatialHashing::getListParticles(int index){
	return hashmap[index];
}

forward_list<Particle*> SpatialHashing::getListForwardNeighbours(int index){
	forward_list<Particle*> listNeighbours;
	for (int i = 0; i < 5; i++){
		if (areCellsNeighbours(index, index+forwardNeighbouringCellIndices[i])){
			forward_list<Particle*>& neighbourCell = hashmap[index+forwardNeighbouringCellIndices[i]];
			listNeighbours.insert_after(listNeighbours.begin(), neighbourCell.begin(), neighbourCell.end());
		}
	}
	return listNeighbours;
}

forward_list<Particle*> SpatialHashing::getListAllNeighbours(int index){
	forward_list<Particle*> listNeighbours;
	for (int i = 0; i < 9; i++){
		if (areCellsNeighbours(index, index+allNeighbouringCellIndices[i])){
			forward_list<Particle*>& neighbourCell = hashmap[index+allNeighbouringCellIndices[i]];
			listNeighbours.insert_after(listNeighbours.begin(), neighbourCell.begin(), neighbourCell.end());
		}
	}
	return listNeighbours;
}

/*inline*/ bool SpatialHashing::areCellsNeighbours(int i, int j){
	return  // not more than a 1-line difference
			(abs(floor(i/nbColumns) - floor(j/nbColumns))<=1 &&
			// not more than a 1-column difference
			abs(i%nbColumns - j%nbColumns)<=1) &&
			// j is a valid
			j < nbColumns*nbRows && j >= 0;
}

void SpatialHashing::addToMap(Particle* p) {
	hashmap[getIndexBox(p->position)].push_front(p);
}


void SpatialHashing::render() {
	glLineWidth(1.0f);
	glColor3f(0,1,0);
	glBegin(GL_LINES);
	for (int i = 1; i < nbRows; i++){
		glVertex3f(bottomLeftCorner.x						, bottomLeftCorner.y+sizeSquare*i, 0);
		glVertex3f(bottomLeftCorner.x+sizeSquare*nbColumns , bottomLeftCorner.y+sizeSquare*i, 0);
	}
	for (int i = 1; i < nbColumns; i++){
		glVertex3f(bottomLeftCorner.x+sizeSquare*i, bottomLeftCorner.y					, 0);
		glVertex3f(bottomLeftCorner.x+sizeSquare*i, bottomLeftCorner.y+sizeSquare*nbRows	, 0);
	}
	glEnd();
}