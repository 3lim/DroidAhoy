#include "headers/controller.h"

Controller::Controller(){
  scope = glfwGetCurrentContext();   
}

Controller::Controller(GLFWwindow * _scope){
  scope = _scope;
  _t_start = clock(); 
}

bool Controller::_apply_input(mat4 & output, float dt){
  return false;
}

bool Controller::apply_input(mat4 & output, float dt){
  bool success = _apply_input(output, dt);
  return success;  
}

float Controller::dt(){
  clock_t _t_end = clock();
  float dt = (((float) _t_end - (float) _t_start) / CLOCKS_PER_SEC ) * 1000;   
  _t_start = clock();
  return dt;
}
