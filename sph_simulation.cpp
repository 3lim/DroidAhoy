#include "headers/sph_simulation.h"
#include <iostream>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

using std::cout;
using std::endl;
using glm::dot;

SPHSimulation::SPHSimulation(const string& parametersFile) : 
  SPHSimulation(SimulationParameters(parametersFile))
{}

SPHSimulation::SPHSimulation(const SimulationParameters& param) : 
  numberParticles(floor(sqrt(param.getNumberParticles()))*floor(sqrt(param.getNumberParticles()))),
  walls({ 
        vec2(param.getSceneWidth()/2, 0.0), 
        vec2(-param.getSceneWidth()/2, 0.0), 
        vec2(0.0, param.getSceneLength()/2), 
        vec2(0.0, -param.getSceneLength()/2) 
      }),
  wallsNormal({ 
              vec2(-1.0, 0.0), 
              vec2(1.0, 0.0), 
              vec2(0.0, -1.0), 
              vec2(0.0, 1.0) 
            }),
  kernelRadius(param.getKernelRadius()),
  massDensity0(param.getMassDensity0()),
  gravity(param.getGravity()),
  rebound(param.getRebound()),
  viscosityScale(param.getViscosityScale()),
  pressureScale(param.getPressureScale()),
  heightOffset(param.getHeightOffset()),
  heightScale(param.getHeightScale())
{
  particles = new Particle[numberParticles];
  if (param.getInitialSetup() == "square"){
    int tmp = sqrt(numberParticles);
    for (int i = 0; i < tmp; i++) {
      for (int j = 0; j < tmp; j++) { 
        particles[i*tmp+j].setPosition(
          vec2(
            -param.getSceneWidth()/2+param.getSceneWidth()*(float)(i+1)/(tmp+1), 
            -param.getSceneLength()/2+param.getSceneLength()*(float)(j+1)/(tmp+1)
          )
        );
      }
    }
  } else if (param.getInitialSetup() == "line"){  
    for (int i = 0; i < numberParticles; i++) {
      particles[i].setPosition(
        vec2(
          -param.getSceneWidth()/2+param.getSceneWidth()*(float)(i+1)/(numberParticles+1), 
          0
        )
      );
    }
  }
}



SPHSimulation::~SPHSimulation(){
  delete[] particles;
}

void SPHSimulation::update(float timeStep){
  setupParticles();
  computeMassDensityAndPressure();
  computeForces();
  integrateParticles(timeStep);
}

time_t last = 0;

void SPHSimulation::setupParticles(){
  /*bool wave = false;
  if (glfwGetTime() - last > 3.0){
    last = glfwGetTime();
    wave = true;
  }*/
  for (int i = 0; i < numberParticles; i++){
    particles[i].massDensity = 0;
    particles[i].setAcceleration(vec2(0.0, 0.0));
    // if (particles[i].getPosition().x < -0.7){
    //   particles[i].setPosition(vec2(0.7, particles[i].getPosition().y));
    //   particles[i].incrAcceleration(vec2(wave ? -200.0 : -200.0, 0.0));
    // }
  }
}

void SPHSimulation::computeMassDensityAndPressure(){
  const float kernelRadius2 = pow(kernelRadius, 2);
  const float cst = 4 / (Pi * pow(kernelRadius, 8));
  for (int i = 0; i < numberParticles; i++){
    for (int j = i; j < numberParticles; j++){
      const vec2 r = particles[i].getPosition() - particles[j].getPosition();
      const float norm2 = dot(r,r);
      if (norm2 >= kernelRadius2) continue;
      const float tmp = cst * pow(kernelRadius2-norm2,3);
      particles[i].massDensity += particles[j].mass*tmp;
      particles[j].massDensity += particles[i].mass*tmp;
    }
    particles[i].pressure = - gravity * (particles[i].massDensity - massDensity0) / massDensity0;
  }
}

void SPHSimulation::computeForces(){
  const float pressureCst = -30 / (Pi * pow(kernelRadius, 5));
  const float viscoCst = 40 / (Pi * pow(kernelRadius, 5));
  const float kernelRadius2 = pow(kernelRadius, 2);

  for (int i = 0; i < numberParticles; i++){
    const float pressureCoef = particles[i].pressure / pow(particles[i].massDensity,2);
    for (int j = i+1; j < numberParticles; j++){
      const vec2 r = particles[i].getPosition() - particles[j].getPosition();
      const float norm2 = dot(r,r);
      if (norm2 >= kernelRadius2) continue;
      const float norm = sqrt(norm2);
      //pressure force
      const float pressureCoef2 = pressureCoef + particles[j].pressure / pow(particles[j].massDensity,2);
      const float tmp = - pressureCoef2 * pressureCst * pow(norm-kernelRadius, 2) / norm;
      const vec2 pressureForce = pressureScale * tmp * r;
      //viscosity force
      const vec2 viscoForce = viscosityScale * (particles[j].getVelocity() - particles[i].getVelocity()) * viscoCst * (kernelRadius - norm) / (particles[i].massDensity * particles[j].massDensity);

      particles[i].incrAcceleration(particles[j].mass * (pressureForce + viscoForce));
      particles[j].incrAcceleration(-particles[i].mass * (pressureForce + viscoForce));
    }
  }
}

void SPHSimulation::integrateParticles(float timeStep){
  for (int i = 0; i < numberParticles; i++){
    // cout << particles[i].getPosition().x << " " << particles[i].getPosition().y << endl;
    particles[i].updateVelocity(timeStep);
    particles[i].updatePosition(timeStep);
    particles[i].height = heightScale * (particles[i].massDensity / massDensity0 + heightOffset);
    for (int j = 0; j < 4; j++){
      // float penetration = dot((particles[i].getPosition() - walls[i]), wallsNormal[i]) - particles[i].radius;
      float penetration = dot((particles[i].getPosition() - walls[j]), wallsNormal[j]);
      if (penetration < 0) {
        float vPen = dot(particles[i].getVelocity(), wallsNormal[j]);
        particles[i].incrPosition(-penetration * wallsNormal[j]);
        particles[i].incrVelocity(-(1 + rebound) * vPen * wallsNormal[j]);
      }
    }
  }
}


void SPHSimulation::render(){
  for (int i = 0; i < numberParticles; i++){
    particles[i].render();
  }
  glPushMatrix();
  glPopMatrix();
}