#pragma once
#include "libs.h"
#include <glm/gtc/type_ptr.hpp>
#include <ctime>

using namespace glm;

class Controls{
  private:
    mat4 mv;
    virtual bool _apply_input(mat4&);
    clock_t t_start;
  public:
    Controls();
    bool apply_input(mat4&);
    mat4 get_mv();

    double time_since_last_input();

  protected:
};
