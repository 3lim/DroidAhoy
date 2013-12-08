#pragma once
#include "libs.h"
#include "transformable.h"
#include <glm/gtc/type_ptr.hpp>
#include <ctime>

using namespace glm;

class Controller{
  public:
    Controller();
    Controller(GLFWwindow *);

  protected:
    GLFWwindow * scope;
};
