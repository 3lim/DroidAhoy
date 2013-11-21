#include "headers/keyboard_controller.h"
#include <glm/gtc/matrix_transform.hpp>

mat4 KeyboardController::m = mat4();
clock_t KeyboardController::start = clock();

KeyboardController::KeyboardController(){}

void KeyboardController::_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
    glfwSetWindowShouldClose(window, GL_TRUE);
  }
  
  vec4 temp = normalize(m * vec4(0.0f, 0.0f, 1.0f, 0.0f));
  vec3 direction = vec3(temp.x, temp.y, temp.z);
  float dt = (float) (((float)clock()-start)/CLOCKS_PER_SEC);
  switch(key){
    case GLFW_KEY_W :
      m = glm::translate(m, 5.0f * direction * dt); 
      break;
    case GLFW_KEY_S :
      m = glm::translate(m, -5.0f * direction * dt ); 
      break;
  }
  start = clock();
}

KeyboardController::KeyboardController(mat4& output, GLFWwindow * window){
  glfwSetKeyCallback(window, _key_callback);
  m = output;
}

bool KeyboardController::_apply_input(mat4& out){
  out = m;
  return true;  
}


