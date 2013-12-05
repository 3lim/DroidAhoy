#include "headers/boat.h"
#include <iostream>

Boat::Boat() {}

Boat::Boat(const Model &m) : model(m)
{}


void Boat::update(){
	model.set_position(position.x, position.y, height+0.001);
}
	
void Boat::setOrientation(const vec3& up){
	// model.set_Orientation(up);
	model.set_rotation(vec3(3.14/2,0,atan(velocity.x/velocity.y)));
	//model.set_rotation(0,0,0);
	model.rotateRad(up.x, -up.y, 0);
	// model.set_rotation(vec3(3.14/2,0,2.0));
	// model.set_rotation(vec3(3.14/2, -up.y, 0));
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
