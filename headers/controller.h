#pragma once
#include "libs.h"
#include <glm/gtc/type_ptr.hpp>
#include <ctime>

using namespace glm;

class Controller{
  private:
    virtual bool _apply_input(mat4&);
    clock_t t_start;
  public:
    Controller();
    mat4 m;
    bool apply_input(mat4&);

    double time_since_last_input();

  protected:
};
