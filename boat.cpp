#include "headers/boat.h"
#include <iostream>

Boat::Boat() {}

Boat::Boat(const Model &m) : model(m)
{}


void Boat::update(){
	model.set_position(position.x, position.y, height+0.001);
}

void Boat::draw(const mat4& m){
	model.draw(m);
}

void Boat::render(){
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	glColor3f(1.0,0.0,0.0);
	glVertex3f(position.x, position.y, height);
	glEnd();
}
