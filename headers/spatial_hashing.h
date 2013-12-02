#ifndef _SPATIALHASHING_
#define _SPATIALHASHING_

#include <map>
#include <forward_list>
#include <array>
#include "particle.h"

using std::array;
using std::map;
using std::forward_list;

class SpatialHashing {
public:
	SpatialHashing() {}
	SpatialHashing(float, float, float);
	void clear();
	int getNumberCells();
	forward_list<Particle*>& getListParticles(int);
	// don't use, it's shit
	forward_list<Particle*> getListForwardNeighbours(int);
	// not that either
	forward_list<Particle*> getListAllNeighbours(int);
	void addToMap(Particle*);
	void render();
	int getIndexBox(const vec2&);
	const array<int,4>& getForwardNeighbouringCellIndices() { return forwardNeighbouringCellIndices; }
	const array<int,9>& getAllNeighbouringCellIndices() { return allNeighbouringCellIndices; }
	bool areCellsNeighbours(int, int);
private:
	map<int, forward_list<Particle*>> hashmap;
	float sizeSquare;
	vec2 bottomLeftCorner;
	int nbRows, nbColumns;
	array<int,4> forwardNeighbouringCellIndices;
	array<int,9> allNeighbouringCellIndices;
};

#endif