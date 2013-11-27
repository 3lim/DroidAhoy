#pragma once
#include "libs.h"
#include <glm/gtc/type_ptr.hpp>
#include <ctime>

using namespace glm;

class Controller{
  private:
    virtual bool _apply_input(mat4&, float);
    clock_t t_start;

  public:
    Controller();
    Controller(GLFWwindow *);
    bool apply_input(mat4&, float);

    float dt();

  protected:
    GLFWwindow * scope;
};
