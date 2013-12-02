#include "engine.h"
#include "keyboard_controller.h"

class AhoyEngine : public Engine{
  private:
    AhoyEngine();
    KeyboardController kb;   
  public:
    ~AhoyEngine();
    AhoyEngine(int width, int height, string name) : Engine(width,height,name){
    }
    int init();
    int _update();
    int render();

  protected:

};
