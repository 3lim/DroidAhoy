#include "headers/controller.h"

Controller::Controller(){
  t_start = clock(); 
}

bool Controller::_apply_input(mat4 & output){
  return false;
}

bool Controller::apply_input(mat4 & output){
  t_start = clock();
  bool success = _apply_input(output);
  return success;  
}

double Controller::time_since_last_input(){
  clock_t t_end = clock();
  double dt = (((double) t_end - (double) t_start) / CLOCKS_PER_SEC ) * 1000;   
  return dt;
}
