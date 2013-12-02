#include "engine.h"
#include "model.h"
#include "sph_simulation.h"
#include "keyboard_controller.h"

class AhoyEngine : public Engine{
  private:
    AhoyEngine();
    SPHSimulation *sim;
    Model *boat;
    KeyboardController kb;
    float timeStep;
  public:
    ~AhoyEngine();
    AhoyEngine(int width, int height, string name) : Engine(width,height,name), sim(NULL), timeStep((float)(1/60.0f)) {}
    int init();
    int update();
    int render();
  protected:

};
