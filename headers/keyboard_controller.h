#pragma once
#include "libs.h"
#include "controller.h"
#include <glm/gtc/type_ptr.hpp>
#include <ctime>

using namespace glm;

class KeyboardController : public Controller{
  private:
    mat4 mv;
    bool _apply_input(mat4&);
  
  public:
    KeyboardController(mat4&);

  protected:
};
