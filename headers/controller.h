#pragma once
#include "libs.h"
#include "transformable.h"
#include <glm/gtc/type_ptr.hpp>
#include <ctime>

using namespace glm;

/* 
 *  Supposed to be generic abstract class to provide
 *  a similar interface to all forms of controllers.
 *  Does not enforce anything right now.
 */
class Controller{
  public:
    Controller();
    Controller(GLFWwindow *);

  protected:
    GLFWwindow * scope;
};
