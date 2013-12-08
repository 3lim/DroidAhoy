#include "engine.h"
#include "model.h"
#include "sph_simulation.h"
#include "keyboard_controller.h"
#include "android_controller.h"

/**
 *  Droid A'hoy's implementation of the 
 *  engine class. Inlcudes the SPH simulation,
 *  android controls and also keyboard controls for
 *  the camera.
 */
class AhoyEngine : public Engine{
  private:
    AhoyEngine();
    SPHSimulation *sim;
    KeyboardController kb;
    AndroidController *ak;
    float timeStep;
  public:
    ~AhoyEngine();
    AhoyEngine(string name) : Engine(name), sim(NULL), timeStep((float)(1/60.0f)) {}
    AhoyEngine(int width, int height, string name, bool f=false) : Engine(width,height,name,f), sim(NULL), timeStep((float)(1/60.0f)) {}
    int init();
    int update();
    int render();
  protected:

};
