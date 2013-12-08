#include "headers/sph_simulation.h"
#include <iostream>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <algorithm>

using std::min;
using std::max;
using std::cout;
using std::endl;
using glm::dot;

SPHSimulation::SPHSimulation(const string& parametersFile) : 
  SPHSimulation(SimulationParameters(parametersFile))
{}

SPHSimulation::SPHSimulation(const SimulationParameters& param) :
  numberParticles(floor(sqrt((float)param.getNumberParticles()))*floor(sqrt((float)param.getNumberParticles()))),
  oceanSurface(param.getMassDensity0(), 50, 50, param.getSceneWidth(), param.getSceneLength(), param.getHeightOffset(), param.getHeightScale()),
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
    int tmp = sqrt((float)numberParticles);
    for (int i = 0; i < tmp; i++) {
      for (int j = 0; j < tmp; j++) { 
        particles[i*tmp+j].position = vec2(
          -sceneWidth/2+sceneWidth*(float)(i+1)/(tmp+1), 
          -sceneLength/2+sceneLength*(float)(j+1)/(tmp+1)
        );
      }
    }
  } else if (param.getInitialSetup() == "line"){  
    for (int i = 0; i < numberParticles; i++) {
      particles[i].position = vec2(
        -sceneWidth/2+sceneWidth*(float)(i+1)/(numberParticles+1), 
        0
      );
    }
  }
  // particles[0].setVelocity(vec2(15, 10));
}



SPHSimulation::~SPHSimulation(){
  delete[] particles;
}

int last = 0;
int jezfe = 2;
void SPHSimulation::update(float timeStep){
  int frequency = 200;
  last++;
  /*if (last % frequency == 0){
    std::cout << "Wave!" << std::endl;
    // if (rand() % 4 < 3)
      createLinearWave((jezfe++)%4, 0.0, 1.0, 800);
    // else
      // createCircularWave(vec2(0.0, 0.0), 0.11, 40);
  }*/
  computeMassDensityAndPressure();
  computeForces();
  spatialHashing.clear();
  integrateParticles(timeStep);
  oceanSurface.update(spatialHashing, kernelRadius);
  for (int i = 0; i < numberParticles; i++){
    particles[i].massDensity = 0;
    particles[i].acceleration = vec2(0.0, 0.0);
  }

  //cout << glfwGetTime() << " " << last << " " << numberParticles << endl;
}

void SPHSimulation::createLinearWave(int wall, float boundary1, float boundary2, float strength){
  bool xDir = walls[wall].x != 0.0;
  vec2 w1(0.85f*walls[wall]);
  vec2 w2(walls[wall]);
  if (w2.x < w1.x || w2.y < w1.y){
    vec2 tmp(w2);
    w2 = w1;
    w1 = tmp;
  }
  float b1 = (xDir ? sceneLength : sceneWidth) * std::min(boundary1, boundary2) - (xDir ? sceneLength : sceneWidth)/2;
  float b2 = (xDir ? sceneLength : sceneWidth) * std::max(boundary1, boundary2) - (xDir ? sceneLength : sceneWidth)/2;
  for (int i = 0; i < numberParticles; i++){
    const vec2& pos = particles[i].position;
    if ((xDir && pos.x > w1.x && pos.x < w2.x && pos.y > b1 && pos.y < b2) || 
      (pos.y > w1.y && pos.y < w2.y && pos.x > b1 && pos.x < b2)){ 
      particles[i].acceleration += strength*wallsNormal[wall];
      // particles[i].velocity = vec2(0,0);
    }
  }
}

void SPHSimulation::createCircularWave(const vec2& center, float radius, float strength){
  const array<int,9>& neighbourIndices = spatialHashing.getAllNeighbouringCellIndices();
  const int cellIndex = spatialHashing.getIndexBox(center);
  const float radius2 = pow(radius, 2);
  const float cst = 4/(Pi*pow(radius,8));
  for (int k = 0; k < 9; k++){
    int neighbourIndex = cellIndex + neighbourIndices[k];
    if (spatialHashing.areCellsNeighbours(cellIndex, neighbourIndex)){
      forward_list<Particle*>& particlesCell = spatialHashing.getListParticles(neighbourIndex);
      for (Particle* p : particlesCell){
        const vec2 r = p->position - center;
        const float norm2 = dot(r,r);
        if (norm2 < radius2) {
          const float tmp = cst * pow(radius2-norm2,3);
          p->acceleration += strength*tmp*glm::normalize(r);
          // p->velocity = vec2(0,0);
        }        
      }
    }
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
      computeDensity(&particles[i], &particles[j]);
    }
    particles[i].pressure = - gravity * (particles[i].massDensity - massDensity0) / massDensity0;
  }
  #else
  const array<int,4>& neighbouringCellIndices = spatialHashing.getForwardNeighbouringCellIndices();
  // for each cell
  for (int cellIndex = 0; cellIndex < spatialHashing.getNumberCells(); cellIndex++){
    forward_list<Particle*>& particlesCell = spatialHashing.getListParticles(cellIndex);
    // for each neighbouring cell (strict neighbour, meaning it excludes itself)
    for (int i = 0; i < 4; i++){
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
    for (int j = i+1; j < numberParticles; j++){
      computeForces(&particles[i], &particles[j]);
    }
  }
  #else
  const array<int,4>& neighbouringCellIndices = spatialHashing.getForwardNeighbouringCellIndices();
  // for each cell
  for (int cellIndex = 0; cellIndex < spatialHashing.getNumberCells(); cellIndex++){
    forward_list<Particle*>& particlesCell = spatialHashing.getListParticles(cellIndex);
    // for each neighbouring cell
    for (int i = 0; i < 4; i++){
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
    particles[i].updateVelocity(timeStep*2);
    particles[i].updatePosition(timeStep);
    particles[i].updateWalls(walls, wallsNormal, rebound);
    particles[i].height = heightScale * (particles[i].massDensity / massDensity0 + heightOffset);
    spatialHashing.addToMap((&particles[i]));
  }
  for (Boat &b : boats){
    vec3 normal3(oceanSurface.interpolateNormalAtPosition(b.position));
    vec2 normal2(-normal3.y, -normal3.x);
    b.velocity += 0.2f*normal2;
    b.velocity *= 0.95f;
    //b.setDirection();
    b.updateVelocity(timeStep);
    // b.setOrientation(normal3);
    b.setOrientation(normal3);
    b.updatePosition(timeStep);
    b.updateWalls(walls, wallsNormal, 1.0f);
    b.height = oceanSurface.interpolateHeightAtPosition(b.position);
    b.update();
  }
}


void SPHSimulation::render(){
  
  glPushMatrix();
   // oceanSurface.render();
   for (int i = 0; i < numberParticles; i++){
     // particles[i].render();
   }
   for (Boat &b : boats){
    b.render();
   }
   spatialHashing.render();
   glBegin(GL_LINE_STRIP);
   glLineWidth(2.0f);
   glColor3f(1,0,0);
     glVertex3f(-sceneWidth/2,-sceneLength/2,0);
     glVertex3f(-sceneWidth/2,sceneLength/2,0);
     glVertex3f(sceneWidth/2, sceneLength/2,0);
     glVertex3f(sceneWidth/2, -sceneLength/2,0);
     glVertex3f(-sceneWidth/2,-sceneLength/2,0);
   glEnd();
  glPopMatrix();
}

void SPHSimulation::draw(const mat4& vp){
  oceanSurface.draw(vp);
  for (Boat &b : boats){
    b.draw(vp);
  }
}
