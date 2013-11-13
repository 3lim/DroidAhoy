#include "headers/controls.h"

Controls::Controls(){
  t_start = clock(); 
}

bool Controls::apply_input(mat4 & output){
  t_start = clock();
  bool success = _apply_input(output);
  return success;  
}

mat4 Controls::get_mv(){
  return mv;  
}

double Controls::time_since_last_input(){
  clock_t t_end = clock();
  double dt = (((double) t_end - (double) t_start) / CLOCKS_PER_SEC ) * 1000;   
  return dt;
}
