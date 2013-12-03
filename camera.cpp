#include "headers/camera.h"

/*
 *  Default constructor for the camera 
 */
Camera::Camera() : Transformable(){
  _t_order_reverse = true;
  projection = perspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
}

/*
 *  Constructor to define a projection for the camera by a mat4.
 */
Camera::Camera(const mat4& _p) : Transformable(){
  _t_order_reverse = true;
  projection = _p;
}

/*
 *  Constructor to define a projection for the camera by a mat4 and a camera transformation
 *  matrix.
 */
Camera::Camera(const mat4& _p, const mat4& _c) : Transformable(){
  _t_order_reverse = true;
  _t_invalid = true;
  projection = _p;
  _ori = toQuat(_c);
  //set_position(vec3(_c[3]));
  //set_rotation(atan2(_c[2][1], _c[2][2]), 
  //             atan2(-_c[2][0], sqrt(_c[2][1]*_c[2][1] + _c[2][2] * _c[2][2])),
  //             atan2(_c[1][0], _c[0][0])); 
}

/*
 *  Getting camera projection matrix.
 */
mat4 Camera::get_view(){
  return projection * get_transformation();
}

vec3 Camera::up(){
  return - _up * _ori;
}

vec3 Camera::forward(){
  return _forward * _ori;
}

vec3 Camera::right(){
  return - _right * _ori;
}
