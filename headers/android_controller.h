#pragma once
#include "sph_simulation.h"
#include "controller.h"
#include "port_listener.h"

using namespace glm;
using namespace std;

class AndroidController : public Controller{
  private:
    thread receiver;
    PortListener pl;
    double last_time;

  public:
    AndroidController();
    ~AndroidController();
    AndroidController(GLFWwindow * w);
    bool apply_input(SPHSimulation*, float);
};
