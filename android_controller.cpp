#include "headers/android_controller.h"

AndroidController::AndroidController() : Controller(){
  receiver = thread(&PortListener::receiveData, PortListener());
  receiver.join();
};

AndroidController::~AndroidController(){
}


AndroidController::AndroidController(GLFWwindow * w) : Controller(w){
  pl = PortListener();
  std::thread receive(&PortListener::receiveData, pl);
  receive.join();
};

bool AndroidController::apply_input(SPHSimulation* s, float dt){
  vec3 rot = pl.getVec();
  
  //s -> createCircularWave(vec2(0.0, 0.0), 0.11, 40);

  if(abs(rot.x) > 45.0f && glfwGetTime() - last_time > 5.0f){
    s -> createLinearWave(0, 0.0, 1.0, 1000);
    last_time = glfwGetTime();
  }

  if(abs(rot.y) > 45.0f && glfwGetTime() - last_time > 5.0f){
    s -> createLinearWave(1, 0.0, 1.0, 1000);
    last_time = glfwGetTime();
  }
 
  if(abs(rot.z) > 45.0f && glfwGetTime() - last_time > 5.0f){
    s-> createCircularWave(vec2(0.0, 0.0), 0.11, 40);
    last_time = glfwGetTime();
  }
  

}

