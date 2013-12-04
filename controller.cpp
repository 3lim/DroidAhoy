#include "headers/controller.h"

Controller::Controller(){
  scope = glfwGetCurrentContext();   
}

Controller::Controller(GLFWwindow * _scope){
  scope = _scope;
}

//bool Controller::_apply_input(mat4 & output, float dt){
//  return false;
//}
//
//bool Controller::apply_input(mat4 & output, float dt){
//  bool success = _apply_input(output, dt);
//  return success;  
//}
//
//float Controller::dt(){
//  clock_t t_end = clock();
//  float dt = (((float) t_end - (float) t_start) / CLOCKS_PER_SEC ) * 1000;   
//  t_start = clock();
//  return dt;
//}
