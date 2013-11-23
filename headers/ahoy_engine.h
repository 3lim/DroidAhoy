#include "engine.h"
#include "sph_simulation.h"

class AhoyEngine : public Engine{
  private:
    AhoyEngine();
    SPHSimulation sim;
    float timeStep;
  public:
    ~AhoyEngine();
    AhoyEngine(int width, int height, string name) : Engine(width,height,name), sim("parameters.txt"), timeStep((float)(1/60.0f)) {}
    int init();
    int update();
    int render();
  protected:

};
