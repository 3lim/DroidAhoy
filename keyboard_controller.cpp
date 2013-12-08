#include "headers/keyboard_controller.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>

clock_t KeyboardController::start = clock();

KeyboardController::KeyboardController(GLFWwindow * window) : Controller(window){}
KeyboardController::KeyboardController():Controller(){}

bool KeyboardController::apply_input(mat4 & output, float dt){
  bool success = _apply_input(output, dt);
  return success;  
}

bool KeyboardController::apply_input(Transformable& out, float dt){
  //Keys
  bool success = false;
  float speed = dt * 1.0f;
  if(glfwGetKey(scope, GLFW_KEY_W)){
    out.translate(0.0f,0.0f,speed);
    success= true;  
  }
  if(glfwGetKey(scope, GLFW_KEY_A)){
    out.translate(-speed,0.0f,0.0f);
    success= true;  
  }
  if(glfwGetKey(scope, GLFW_KEY_S)){
    out.translate(0.0f,0.0f,-speed);
    success= true;  
  }
  if(glfwGetKey(scope, GLFW_KEY_D)){
    out.translate(speed,0.0f,0.0f);
    success= true;  
  }
  if(glfwGetKey(scope, GLFW_KEY_Q)){
    out.translate(0.0f,-speed,0.0f);
    success= true;  
  }
  if(glfwGetKey(scope, GLFW_KEY_E)){
    out.translate(0.0f,speed,0.0f);
    success= true;  
  }

  //Mouse
  if(glfwGetMouseButton(scope, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
    int w, h;
    double x_pos, y_pos;
    glfwGetWindowSize(scope, &w, &h);
    glfwGetCursorPos(scope, &x_pos, &y_pos);

    double xcen = w - (double) w/2;
    double ycen = h - (double) h/2;

    //x & y distance [-1, 1] relative to the center
    double x = (x_pos - xcen)/xcen;
    double y = (y_pos - ycen)/ycen;

    double xd = (double) dt * x * 60.0f; 
    double yd = (double) dt * y * 60.0f;
    
    out.rotate(yd,xd,0);
    success = true;
  }
  return success;  

}

using namespace std;
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
  if(glfwGetMouseButton(scope, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
    int w, h;
    double x_pos, y_pos;
    glfwGetWindowSize(scope, &w, &h);
    glfwGetCursorPos(scope, &x_pos, &y_pos);

    double xcen = w - (double) w/2;
    double ycen = h - (double) h/2;

    //x & y distance [-1, 1] relative to the center
    double x = (x_pos - xcen)/xcen;
    double y = (y_pos - ycen)/ycen;

    double xd = (double) dt * x * 60.0f;
    double yd = (double) dt * y * 60.0f;
    mat4 xrot = mat4(1.0f);
    mat4 yrot = mat4(1.0f); 
   
    xrot = rotate(xrot, (mediump_float) xd, vec3(0.0f, 1.0f, 0.0f)); 
    yrot = rotate(yrot, (mediump_float) yd, vec3(1.0f, 0.0f, 0.0f)); 

    out = xrot*yrot*out;
    
    success = true;
  }
  return success;  
}
