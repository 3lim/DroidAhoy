#include "engine.h"
#include "simulation.h"

class AhoyEngine : public Engine{
  private:
    AhoyEngine();
    Simulation sim;
    float timeStep;
  public:
    ~AhoyEngine();
    AhoyEngine(int width, int height, string name) : Engine(width,height,name), sim(1000), timeStep((float)(1/60.0f)) {
    }
    int init();
    int update();
    int render();

  protected:

};
