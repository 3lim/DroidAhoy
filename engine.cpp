#include "headers/engine.h"
#include <string>
#include <iostream>

Engine::~Engine(){}

int Engine::run(void)
{
  /* Giving output about OpenGL Context */
  /* Initialize the library */
  if (!glfwInit())
    return -1;

  /* Create a windowed mode window and its OpenGL context */
  glfwWindowHint(GLFW_SAMPLES, 4);
  
  GLFWmonitor * win = glfwGetPrimaryMonitor();
  const GLFWvidmode * screen = glfwGetVideoMode(win);

  if(w_window == -1)
    w_window = screen -> width;
  if(h_window == -1)
    h_window = screen -> height;

  if(fullscreen)
   window = glfwCreateWindow(w_window, h_window, n_window.c_str(), glfwGetPrimaryMonitor(), NULL); // Fullscreen
  else
   window = glfwCreateWindow(w_window, h_window, n_window.c_str(), NULL, NULL); // Windowed
      
  if (!window){
    glfwTerminate();
    cerr << "Could not open window" << endl;
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  GLint majorGLVersion, minorGLVersion;
  glGetIntegerv(GL_MAJOR_VERSION, &majorGLVersion);
  glGetIntegerv(GL_MINOR_VERSION, &minorGLVersion);
  cout << "OpenGL version: " << majorGLVersion << "." << minorGLVersion << endl;
  
  /* Initialize engine */
  init();

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window)){
    /* Render here */
    update();
    render();

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}


