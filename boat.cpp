#include "headers/boat.h"
#include <iostream>

Boat::Boat(const Model &m) : model(m)
{} 

void Boat::update(){
	model.set_position(position.x, position.y, height+0.01);
}

void Boat::draw(const mat4& m){
	model.draw(m);	
}