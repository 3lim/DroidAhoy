#include "headers/model.h"
#include <iostream>

using namespace std;

Transformable::Transformable(){
  t_invalid = false;
  pt = mat4(1.0f);
  up = vec3(0.0f, 1.0f, 0.0f);
  right = vec3(1.0f, 0.0f, 0.0f);
  forward = vec3(0.0f, 0.0f, 1.0f);
  _scale = 1.0f;
}

float Transformable::get_scale(){
  return _scale;
}

void Transformable::set_scale(float new_scale){
  t_invalid = true;
  _scale = new_scale;
}

const mat4& Transformable::get_transformation(){
  if(t_invalid){
    //Form rotation
    mat4 rotX(1.0f), rotY(1.0f), rotZ(1.0f), rota;
    rotX = rotate(rotX, rot.x, right);
    rotY = rotate(rotY, rot.y, up);
    rotZ = rotate(rotZ, rot.z, forward);
    rota = rotZ * rotY * rotX;
    
    //Update up and right
    //up = vec3(rota * vec4(0,1.0f,0,0));
    //right = vec3(rota * vec4(1.0f,0,0,0));
    //forward = cross(right, up); 
    //
    //cout << endl << endl;
    //cout << "up:\t"     << up.x << ", "       << up.y << ", " << up.z << endl;
    //cout << "right:\t"  << right.x << ", "    << right.y      << ", " << right.z << endl;
    //cout << "forward:\t"<< forward.x  << ", " << forward.y << "," << forward.z << endl;

    //Form translation
    mat4 transX(1.0f), transY(1.0f), transZ(1.0f), transa;
    transX = translate(transX, pos.x*right);
    transY = translate(transY, pos.y*up);
    transZ = translate(transZ, pos.z*forward);
    transa = transZ * transY * transX;

    mat4 scaler(1.0f);
    scaler = glm::scale(scaler, vec3(_scale));

    //Total transformation
    pt = scaler * transa * rota;

    t_invalid = false;
    return pt;
  }
  else{
    return pt;
  }
}

void Transformable::set_rotation(float ax, float ay, float az){
  t_invalid = true;
  rot.x = ax;
  rot.y = ay;
  rot.z = az;
}

void Transformable::set_rotation(vec3 r){
  t_invalid = true;
  rot = r;
}

void Transformable::add_rotation(float ax, float ay, float az){
  t_invalid = true;
  rot.x += ax;
  rot.y += ay;
  rot.z += az;
}

const vec3& Transformable::get_rotation(){
  return rot;
}

void Transformable::set_position(float x, float y, float z){
  t_invalid = true;
  pos.x = x;
  pos.y = y;
  pos.z = z;
}

void Transformable::set_position(vec3 p){
  t_invalid = true;
  pos = p;
}

void Transformable::add_position(float x, float y, float z){
  t_invalid = true;
  pos.x += x;
  pos.y += y;
  pos.z += z;
}

const vec3& Transformable::get_position(){
  return pos;
}


/*

   The up and right are used to determine around which axises to perform
   rotations and translations.

 */
void Transformable::set_up(float x, float y, float z){
  t_invalid = true;
  up = vec3(x,y,z);
}

void Transformable::set_up(vec3 _up){
  t_invalid = true;
  up = _up;
}

const vec3& Transformable::get_up(){
  return up;
}

void Transformable::set_right(vec3 _right){
  t_invalid = true;
  right = _right;
}

void Transformable::set_right(float x, float y, float z){
  t_invalid = true;
  right = vec3(x,y,z);
}

const vec3& Transformable::get_right(){
  return right;
}
