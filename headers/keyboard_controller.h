#pragma once
#include "libs.h"
#include "controller.h"
#include <glm/gtc/type_ptr.hpp>
#include <ctime>

using namespace glm;

class KeyboardController : public Controller{
  private:
    bool _apply_input(mat4&, float);
    static clock_t start;
  public:
    bool apply_input(Transformable&, float);
    bool apply_input(mat4&, float);
    KeyboardController(GLFWwindow*);
    KeyboardController();

  protected:
};
