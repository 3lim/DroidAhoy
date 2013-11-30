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
  oceanSurface(param.getMassDensity0(), 20, 20, param.getSceneWidth(), param.getSceneLength(), param.getHeightOffset(), param.getHeightScale()),
  spatialHashing(param.getKernelRadius(), param.getSceneWidth(), param.getSceneLength()),
  sceneWidth(param.getSceneWidth()),
  sceneLength(param.getSceneLength()),
  walls({ 
    vec2(sceneWidth/2, 0.0), 
    vec2(-sceneWidth/2, 0.0), 
    vec2(0.0, sceneLength/2), 
    vec2(0.0, -sceneLength/2) 
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
        particles[i*tmp+j].position = vec2(
          -sceneWidth/2+sceneWidth*(float)(i+1)/(tmp+1), 
          -sceneLength/2+sceneLength*(float)(j+1)/(tmp+1));
      }
    }
  } else if (param.getInitialSetup() == "line"){  
    for (int i = 0; i < numberParticles; i++) {
      particles[i].position = vec2(
          -sceneWidth/2+sceneWidth*(float)(i+1)/(numberParticles+1), 
          0);
    }
  }
  // particles[0].setVelocity(vec2(15, 10));
}



SPHSimulation::~SPHSimulation(){
  delete[] particles;
}

int last = 0;

void SPHSimulation::update(float timeStep){
  setupParticles();
  if (last % 80 == 0){
  for (int i = 0; i < numberParticles; i++){
    if (particles[i].position.y > sceneWidth/2*0.8){
      particles[i].acceleration.y = -200;
    }
  }
    
  }
  // for (int i = 0; i < numberParticles; i++){
  //   if (particles[i].position.y < -sceneWidth/2*0.8){
  //     particles[i].position.y = sceneWidth/2;
  //     particles[i].acceleration.y = -5000;
  //   }
  // }
  computeMassDensityAndPressure();
  computeForces();
  integrateParticles(timeStep);
  oceanSurface.update(spatialHashing, kernelRadius);
  spatialHashing.clear();
  cout << glfwGetTime() << " " << last << " " << numberParticles << endl;
  last++;
}


void SPHSimulation::setupParticles(){
  /*bool wave = false;
  if (glfwGetTime() - last > 3.0){
    last = glfwGetTime();
    wave = true;
  }*/
  for (int i = 0; i < numberParticles; i++){
    particles[i].massDensity = 0;
    particles[i].acceleration = vec2(0.0, 0.0);
    spatialHashing.addToMap((particles+i));
    // if (last == 0)
    //   particles[i].acceleration += (vec2(8000.0, 1400.0));
    // if (particles[i].position.x < -0.7){
    //   particles[i].position = vec2(0.7, particles[i].position.y);
    //   particles[i].acceleration += (vec2(wave ? -200.0 : -200.0, 0.0));
    // }
  }
}

#define HASHMAP

class DensityComputer{
public:
  DensityComputer(float kernelRadius) :
    kernelRadius2(pow(kernelRadius, 2)),
    cst(4/(Pi*pow(kernelRadius,8)))
  {}
  void operator()(Particle* p1, Particle* p2){
    const vec2 r = p1->position - p2->position;
    const float norm2 = dot(r,r);
    if (norm2 < kernelRadius2) {
      const float tmp = cst * pow(kernelRadius2-norm2,3);
      p1->massDensity += p2->mass*tmp;
      p2->massDensity += p1->mass*tmp;
    }
  }
private:
  const float kernelRadius2;
  const float cst;
};

void SPHSimulation::computeMassDensityAndPressure(){
  DensityComputer computeDensity(kernelRadius);

  #ifndef HASHMAP
  for (int i = 0; i < numberParticles; i++){
    for (int j = i; j < numberParticles; j++){
      computeDensity(particles[i], particles[j]);
    }
    particles[i].pressure = - gravity * (particles[i].massDensity - massDensity0) / massDensity0;
  }
  #else
  const array<int,4>& neighbouringCellIndices = spatialHashing.getForwardNeighbouringCellIndices();
  // for each cell
  for (int cellIndex = 0; cellIndex < spatialHashing.getNumberCells(); cellIndex++){
    forward_list<Particle*>& particlesCell = spatialHashing.getListParticles(cellIndex);
    // for each neighbouring cell (strict neighbour, meaning it excludes )
    for (int i = 0; i < neighbouringCellIndices.size(); i++){
      int neighbourIndex = cellIndex+neighbouringCellIndices[i];
      // check that the neighbouring cell exists
      if (spatialHashing.areCellsNeighbours(cellIndex, neighbourIndex)){
        // for each particle in the main cell
        forward_list<Particle*>& particlesNeighbouringCell = spatialHashing.getListParticles(neighbourIndex);
        for (Particle* p : particlesCell){
          // for each particle in the neighbouring cell
          for (Particle* n : particlesNeighbouringCell){
            computeDensity(p, n);
          }
        }
      }
    }
    for (forward_list<Particle*>::iterator itp = particlesCell.begin(); itp != particlesCell.end(); ++itp){
      for (forward_list<Particle*>::iterator itn(itp); itn != particlesCell.end(); ++itn){
        computeDensity(*itp, *itn);
      }
      (*itp)->pressure = - gravity * ((*itp)->massDensity - massDensity0) / massDensity0;
    }
  }
  #endif
}

class ForcesComputer{
public:
  ForcesComputer(float kernelRadius, float viscosityScale, float pressureScale) :
    kernelRadius(kernelRadius),
    kernelRadius2(pow(kernelRadius, 2)),
    viscosityScale(viscosityScale),
    pressureScale(pressureScale),
    pressureCst(-30 / (Pi * pow(kernelRadius, 5))),
    viscoCst(40 / (Pi * pow(kernelRadius, 5)))
  {}
  void operator()(Particle* p1, Particle* p2){
    const vec2 r = p1->position - p2->position;
    const float norm2 = dot(r,r);
    if (norm2 < kernelRadius2){
      const float norm = sqrt(norm2);
      //pressure force
      const float pressureCoef = p1->pressure / pow(p1->massDensity,2) + p2->pressure / pow(p2->massDensity,2);
      const float tmp = - pressureCoef * pressureCst * pow(norm-kernelRadius, 2) / norm;
      const vec2 pressureForce = pressureScale * tmp * r;
      //viscosity force
      const vec2 viscoForce = viscosityScale * (p2->velocity - p1->velocity) * viscoCst * (kernelRadius - norm) / (p1->massDensity * p2->massDensity);

      p1->acceleration += p2->mass * (pressureForce + viscoForce);
      p2->acceleration += -p1->mass * (pressureForce + viscoForce);
    }
  }
private:
  const float kernelRadius;
  const float kernelRadius2;
  const float viscosityScale;
  const float pressureScale;
  const float pressureCst;
  const float viscoCst;
};

void SPHSimulation::computeForces(){
  ForcesComputer computeForces(kernelRadius, viscosityScale, pressureScale);

  #ifndef HASHMAP
  for (int i = 0; i < numberParticles; i++){
    const float pressureCoef = particles[i].pressure / pow(particles[i].massDensity,2);
    for (int j = i+1; j < numberParticles; j++){
      computeForces(particles[i], particles[j]);
    }
  }
  #else
  const array<int,4>& neighbouringCellIndices = spatialHashing.getForwardNeighbouringCellIndices();
  // for each cell
  for (int cellIndex = 0; cellIndex < spatialHashing.getNumberCells(); cellIndex++){
    forward_list<Particle*>& particlesCell = spatialHashing.getListParticles(cellIndex);
    // for each neighbouring cell
    for (int i = 0; i < neighbouringCellIndices.size(); i++){
      int neighbourIndex = cellIndex+neighbouringCellIndices[i];
      if (spatialHashing.areCellsNeighbours(cellIndex, neighbourIndex)){
        // for each particle in the main cell
        forward_list<Particle*>& particlesNeighbouringCell = spatialHashing.getListParticles(neighbourIndex);
        for (Particle* p : particlesCell){
          // for each particle in the neighbouring cell
          for (Particle* n : particlesNeighbouringCell){
            computeForces(p, n);
          }
        }
      }
    }
    for (forward_list<Particle*>::iterator itp = particlesCell.begin(); itp != particlesCell.end(); ++itp){
      for (forward_list<Particle*>::iterator itn = itp; ++itn != particlesCell.end(); /**/){
        computeForces(*itp, *itn);
      }
    }
  }
  #endif
}

void SPHSimulation::integrateParticles(float timeStep){
  for (int i = 0; i < numberParticles; i++){
    particles[i].updateVelocity(timeStep);
    particles[i].updatePosition(timeStep);
    particles[i].height = heightScale * (particles[i].massDensity / massDensity0 + heightOffset);
    for (int j = 0; j < 4; j++){
      float penetration = dot((particles[i].position - walls[j]), wallsNormal[j]);
      if (penetration < 0) {
        float vPen = dot(particles[i].velocity, wallsNormal[j]);
        particles[i].position += -penetration * wallsNormal[j];
        particles[i].velocity += -(1 + rebound) * vPen * wallsNormal[j];
      }
    }
  }
}


void SPHSimulation::render(){
  
  oceanSurface.render();
  // for (int i = 0; i < numberParticles; i++){
  //   particles[i].render();
  // }
  // spatialHashing.render();
  // glBegin(GL_LINE_STRIP);
  // glLineWidth(2.0f);
  // glColor3f(1,0,0);
  //   glVertex3f(-sceneWidth/2,-sceneLength/2,0);
  //   glVertex3f(-sceneWidth/2,sceneLength/2,0);
  //   glVertex3f(sceneWidth/2, sceneLength/2,0);
  //   glVertex3f(sceneWidth/2, -sceneLength/2,0);
  //   glVertex3f(-sceneWidth/2,-sceneLength/2,0);
  // glEnd();
  glPopMatrix();
}

void SPHSimulation::draw(const mat4& vp){
  oceanSurface.draw(vp);
}