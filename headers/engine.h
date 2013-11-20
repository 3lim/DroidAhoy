#pragma once
#include "libs.h"
#include <string>

using namespace std;

class Engine{
  private:
    Engine& operator=(Engine&) = delete;
    
  public:
    virtual ~Engine();
    virtual int init()=0;
    virtual int render()=0;
    virtual int update()=0;
    int run(); 

  protected:
    Engine(int width, int height, string name) : w_window(width), h_window(height), n_window(name)
    {}

    const int w_window, h_window;
    const string n_window;
    GLFWwindow* window;

};
