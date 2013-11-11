#include "headers/ahoy_engine.h"

AhoyEngine::~AhoyEngine(){}

int AhoyEngine::init(){
  return -1;
}

int AhoyEngine::update(){
  return -1;
}

int AhoyEngine::render(){
  return -1;
}

int main(){
  AhoyEngine ahoy = AhoyEngine(640, 480, "Ahoy World!");
  ahoy.run();
}
