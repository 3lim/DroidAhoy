#include "engine.h"

class AhoyEngine : public Engine{
  private:
    AhoyEngine();
   
  public:
    ~AhoyEngine();
    AhoyEngine(int width, int height, string name) : Engine(width,height,name){
    }
    int init();
    int update();
    int render();

  protected:

};
