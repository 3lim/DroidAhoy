#include "headers/android_controller.h"

enum { NORTH=0, SOUTH=1, WEST=2, EAST=3 };

AndroidController::AndroidController() : Controller(){
  receiver = thread(&PortListener::receiveData, PortListener());
//  receiver.join();
  last_time = glfwGetTime();
};

AndroidController::~AndroidController(){
}


AndroidController::AndroidController(GLFWwindow * w) : Controller(w){
  //receiver = thread(&PortListener::receiveData, PortListener());
  //receive.join();
};

bool AndroidController::apply_input(SPHSimulation* s, float dt){
  vec3 rot = pl.getVec();
  bool success = false;
  //s -> createCircularWave(vec2(0.0, 0.0), 0.11, 40);
  int direction;
  double temp = last_time;
  //if(abs(rot.x) > 45.0f && glfwGetTime() - last_time > 5.0f){
  if(abs(rot.x) > 10.0f && glfwGetTime() - last_time > 2.0f){
    direction = NORTH;
    if(rot.x < 0 ) direction = SOUTH;
   
    cout << "Recieved: " << rot.x << ", " << rot.y  << std::endl;
    cout << "CREATING " << (direction == NORTH ? "NORTH" : "SOUTH")  <<" WAVE FROM INPUT" << endl;
    s -> createLinearWave(direction, 0.0, 1.0, abs(rot.x)/360.0f * 1200.0f);
    temp = glfwGetTime();
    success = true;
  }
  //if(abs(rot.y) > 45.0f && glfwGetTime() - last_time > 5.0f){
  if(abs(rot.y) > 10.0f && glfwGetTime() - last_time > 2.0f){
    direction = EAST;
    if(rot.y < 0 ) direction = WEST;
    
    cout << "Recieved: " << rot.x << ", " << rot.y  << std::endl;
    cout << "CREATING " << (direction == EAST ? "EAST" : "WEST")  << " WAVE FROM INPUT" << endl;
    s -> createLinearWave(direction, 0.0, 1.0, abs(rot.y)/360.0f * 1200.0f);
    temp = glfwGetTime();
    cout << endl;
    success = true;
  }
  last_time = temp;
  return success;
  ////if(abs(rot.z) > 45.0f && glfwGetTime() - last_time > 5.0f){
  //if(abs(rot.z) > 25.0f){
  //  s-> createCircularWave(vec2(0.0, 0.0), 0.11, 40);
  //  last_time = glfwGetTime();
  //}
  

}

