#include "headers/keyboard_controller.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
//#include <iostream>

mat4 KeyboardController::m = mat4();
clock_t KeyboardController::start = clock();

KeyboardController::KeyboardController(GLFWwindow * window) : Controller(window){}
KeyboardController::KeyboardController():Controller(){}

//using namespace std;
bool KeyboardController::_apply_input(mat4& out, float dt){


  //Keys
  bool success = false;
  if(glfwGetKey(scope, GLFW_KEY_W)){
    out = translate(out, vec3(dt*vec4(0.0f,0.0f,5.0f,0.0f)*out));
    success= true;  
  }
  if(glfwGetKey(scope, GLFW_KEY_A)){
    out = translate(out, vec3(dt*vec4(5.0f,0.0f,0.0f,0.0f)*out));
    success= true;  
  }
  if(glfwGetKey(scope, GLFW_KEY_S)){
    out = translate(out, vec3(dt*vec4(0.0f,0.0f,-5.0f,0.0f)*out));
    success= true;  
  }
  if(glfwGetKey(scope, GLFW_KEY_D)){
    out = translate(out, vec3(dt*vec4(-5.0f,0.0f,0.0f,0.0f)*out));
    success= true;  
  }

  //Mouse
  //TODO

  return success;  
}


