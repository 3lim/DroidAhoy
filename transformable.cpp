#include "headers/model.h"
#include <iostream>

using namespace std;

Transformable::Transformable(){
  t_invalid = false;
  t_order_reverse = false;
  _pt = mat4(1.0f);
  _up = vec3(0.0f, 1.0f, 0.0f);
  _right = vec3(1.0f, 0.0f, 0.0f);
  _forward = vec3(0.0f, 0.0f, -1.0f);
  _ori = quat(vec3(0,0,0));
  _scale = 1.0f;
}

float Transformable::get_scale(){
  return _scale;
}

vec3 Transformable::up(){
  return _up * inverse(_ori);
}

vec3 Transformable::forward(){
  return _forward * inverse(_ori);
}

vec3 Transformable::right(){
  return  _right * inverse(_ori);
}

void Transformable::set_scale(float new_scale){
  t_invalid = true;
  _scale = new_scale;
}

const mat4& Transformable::get_transformation(){
  if(t_invalid){
    //Form translation
    mat4 transX(1.0f), transY(1.0f), transZ(1.0f), transa;
    transX = glm::translate(transX, pos.x*_right);
    transY = glm::translate(transY, pos.y*_up);
    transZ = glm::translate(transZ, pos.z*_forward);
    transa = transZ * transY * transX;

    mat4 scaler(1.0f);
    scaler = glm::scale(scaler, vec3(_scale));

    //Total transformation
    if(!t_order_reverse) _pt = scaler * transa * toMat4(_ori);
    else _pt = scaler * toMat4(_ori) * transa;
    t_invalid = false;
    return _pt;
  }
  else{
    return _pt;
  }
}

void Transformable::set_rotation(float ax, float ay, float az){
  _ori = quat(vec3(ax,ay,az));
  t_invalid = true;
}

void Transformable::set_rotation(vec3 r){
  _ori = quat(r);
  t_invalid = true;
}

void Transformable::rotate(float ax, float ay, float az){
  _ori = quat(vec3(ax,ay,az)) * _ori;
  t_invalid = true;
}

const vec3 Transformable::get_rotation(){
  return eulerAngles(_ori);
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

void Transformable::translate(float x, float y, float z){
  t_invalid = true;
  pos += x * right();
  pos += y * up();
  pos += z * forward();
}

const vec3& Transformable::get_position(){
  return pos;
}


/*

   The up and _right are used to determine around which axises to perform
   rotations and translations.

 */
void Transformable::set_up(float x, float y, float z){
  t_invalid = true;
  _up = vec3(x,y,z);
  _forward = cross(_right,_up);
}

void Transformable::set_up(vec3 __up){
  t_invalid = true;
  _up = __up;
  _forward = cross(_right,_up);
}

const vec3& Transformable::get_up(){
  return _up;
}

void Transformable::set_right(vec3 __right){
  t_invalid = true;
  _right = __right;
  _forward = cross(_right,_up);
}

void Transformable::set_right(float x, float y, float z){
  t_invalid = true;
  _right = vec3(x,y,z);
  _forward = cross(_right,_up);
}

const vec3& Transformable::get_right(){
  return _right;
}
