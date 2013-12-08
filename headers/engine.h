#pragma once
#include "libs.h"
#include "camera.h"
#include <string>

using namespace std;

/**
 *
 *  Engine class to be extended when building base
 *  engine flow (init, update, render). Initializes
 *  OpenGL (glew on linux) through glfw3.
 *
 */
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
    Engine(string name) : w_window(-1), h_window(-1), n_window(name), fullscreen(true)
    {}
    
    Engine(int width, int height, string name, bool f=false) : w_window(width), h_window(height), n_window(name), fullscreen(f)
    {}
    

    int w_window, h_window;
    const string n_window;
    const bool fullscreen;
    Camera cam;
    GLFWwindow* window;

};
