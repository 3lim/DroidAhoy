#include "headers/transformable.h"
#include <iostream>

using namespace std;

Transformable::Transformable(){
  _t_invalid = false;
  _t_order_reverse = false;
  _pt = mat4(1.0f);
  _up = vec3(0.0f, 1.0f, 0.0f);
  _right = vec3(1.0f, 0.0f, 0.0f);
  _forward = vec3(0.0f, 0.0f, 1.0f);
  _ori = quat(vec3(0.0f,0.0f,0.0f));
  _scale = 1.0f;
}

float Transformable::get_scale(){
  return _scale;
}

vec3 Transformable::up(){
  return _up * _ori;
}

vec3 Transformable::forward(){
  return _forward * _ori;
}

vec3 Transformable::right(){
  return  _right * _ori;
}

void Transformable::set_scale(float new_scale){
  _t_invalid = true;
  _scale = new_scale;
}

const mat4& Transformable::get_transformation(){
  if(_t_invalid){
    //Form translation
    mat4 transX(1.0f), transY(1.0f), transZ(1.0f), transa;
    transX = glm::translate(transX, _pos.x*_right);
    transY = glm::translate(transY, _pos.y*_up);
    transZ = glm::translate(transZ, _pos.z*_forward);
    transa = transZ * transY * transX;

    mat4 scaler(1.0f);
    scaler = glm::scale(scaler, vec3(_scale));

    //Total transformation
    if(!_t_order_reverse) 
      _pt = transa * toMat4(_ori) * scaler;
    else 
      _pt = scaler * toMat4(_ori) * transa;
    
    _t_invalid = false;
    return _pt;
  } else {
    return _pt;
  }
}


/*
 * 
 *  Code taken from 
 *  http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/#How_do_I_find_the_rotation_between_2_vectors__
 *
 */
void Transformable::rotate_from_towards(const vec3& from, const vec3& to){
    _t_invalid = true;
    vec3 start = normalize(from);
    vec3 dest = normalize(to);
 
    float cosTheta = dot(start, dest);
    vec3 rotationAxis;
 
    if (cosTheta < -1 + 0.001f){
        // special case when vectors in opposite directions:
        // there is no "ideal" rotation axis
        // So guess one; any will do as long as it's perpendicular to start
        /*rotationAxis = cross(vec3(0.0f, 0.0f, 1.0f), start);
        if (gtx::norm::length2(rotationAxis) < 0.01 ) // bad luck, they were parallel, try again!
            rotationAxis = cross(vec3(1.0f, 0.0f, 0.0f), start);
 
        rotationAxis = normalize(rotationAxis);
        //return gtx::quaternion::angleAxis(180.0f, rotationAxis);
        
        _ori = gtx::quaternion::angleAxis(180.0f, rotationAxis);// * ori;*/
    }
 
    rotationAxis = cross(start, dest);
 
    float s = sqrt( (1+cosTheta)*2 );
    float invs = 1 / s;
 
    _ori = quat(s * 0.5f, rotationAxis.x * invs, rotationAxis.y * invs, rotationAxis.z * invs);// * ori;
    //ori = quat(s * 0.5f, rotationAxis.x * invs, rotationAxis.y * invs, rotationAxis.z * invs) * ori;
}

void Transformable::set_rotation(float ax, float ay, float az){
  ax = radians(ax); ay = radians(ay); az = radians(az);
  _ori = quat(vec3(ax,ay,az));
  _t_invalid = true;
}

void Transformable::set_rotation(const quat& orientation){
  _ori = orientation;
  _t_invalid = true;
}

void Transformable::set_rotation(vec3 r){
  _ori = quat(r);
  _t_invalid = true;
}

void Transformable::rotate(float ax, float ay, float az){
  vec3 angles(radians(ax),radians(ay),radians(az));
  _ori = quat(angles) * _ori;
  _t_invalid = true;
}

void Transformable::rotateRad(float ax, float ay, float az){
  vec3 angles(ax,ay,az);
  _ori = quat(angles) * _ori;
  _t_invalid = true;
}

const vec3 Transformable::get_rotation(){
  return eulerAngles(_ori);
}

void Transformable::set_position(float x, float y, float z){
  _t_invalid = true;
  _pos.x = x;
  _pos.y = y;
  _pos.z = z;
}

void Transformable::set_position(const vec3& p){
  _t_invalid = true;
  _pos = p;
}

void Transformable::translate(float x, float y, float z){
  _t_invalid = true;
  _pos += x * right();
  _pos += y * up();
  _pos += z * forward();
}

const vec3& Transformable::get_position(){
  return _pos;
}


/*

   The up and _right are used to determine around which axises to perform
   rotations and translations.

 */
void Transformable::set_up(float x, float y, float z){
  _t_invalid = true;
  _up = vec3(x,y,z);
  _forward = cross(_right,_up);
}

void Transformable::set_up(const vec3& __up){
  _t_invalid = true;
  _up = __up;
  _forward = cross(_right,_up);
}

const vec3& Transformable::get_up(){
  return _up;
}

void Transformable::set_right(const vec3& __right){
  _t_invalid = true;
  _right = __right;
  _forward = cross(_right,_up);
}

void Transformable::set_right(float x, float y, float z){
  _t_invalid = true;
  _right = vec3(x,y,z);
  _forward = cross(_right,_up);
}

const vec3& Transformable::get_right(){
  return _right;
}
