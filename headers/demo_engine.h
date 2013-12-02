#include "engine.h"
#include "keyboard_controller.h"

class DemoEngine : public Engine{
  private:
    DemoEngine();
    KeyboardController kb;   
  public:
    ~DemoEngine();
    DemoEngine(int width, int height, string name) : Engine(width,height,name){
    }
    int init();
    int update();
    int render();

  protected:

};
