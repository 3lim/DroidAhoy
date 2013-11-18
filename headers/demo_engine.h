#include "engine.h"

class DemoEngine : public Engine{
  private:
    DemoEngine();
   
  public:
    ~DemoEngine();
    DemoEngine(int width, int height, string name) : Engine(width,height,name){
    }
    int init();
    int update();
    int render();

  protected:

};
