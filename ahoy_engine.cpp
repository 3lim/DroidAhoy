#include "headers/primitives.h"
#include "headers/ahoy_engine.h"
#include "headers/shader_manager.h"
#include "headers/obj_loader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

AhoyEngine::~AhoyEngine(){}

GLuint vb;
GLuint mvp_id;
GLuint elementbuffer;
vector<float> boat_vertices = vector<float>();
vector<unsigned> boat_indices = vector<unsigned>();
vec3 boat_center;
glm::vec3 camPos(4,2,2);

int AhoyEngine::init(){
  if (GLEW_OK != glewInit())
  {
      std::cout << "GLEW failed!" << std::endl;
      exit(1);
  }
  using namespace std;
  //glfwSetKeyCallback(window, key_callback );

  ShaderManager::load_program("shaders/basic");
  
  //Load boat
  //string boat_path = "models/boat.obj";
  //ModelGroup boat = OBJLoader::load_model(boat_path);
  //boat_center = OBJLoader::get_approx_center(boat);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_MULTISAMPLE);

	glGenBuffers(1, &vb);
	glBindBuffer(GL_ARRAY_BUFFER, vb);

  //Converting model vertices to one array buffer
  //for(unsigned i=0; i < boat.mdls.size(); i++){
  //  for(unsigned j=0; j<boat.mdls[i].v.size(); j++)
  //    boat_vertices.push_back(boat.mdls[i].v[j]);
  //  for(unsigned j=0; j<boat.mdls[i].indices.size(); j++) 
  //    boat_indices.push_back(boat.mdls[i].indices[j]);
  //}
  sim.update_vertices();
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * sim.numberParticles * 3, &sim.particle_vertices[0], GL_DYNAMIC_DRAW); 

  //glGenBuffers(1, &elementbuffer);
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
  //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * boat_indices.size(), &boat_indices[0], GL_STATIC_DRAW);
  
  mvp_id = glGetUniformLocation(ShaderManager::get_program("basic"), "MVP");
  
  return 1;
}

glm::mat4 Model = glm::mat4(1.0f);  // Changes for each model !

int AhoyEngine::update(){
  sim.update(timeStep);
  sim.update_vertices();
  glBindBuffer(GL_ARRAY_BUFFER, vb);
  float* bufferCopy = (float*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
  glBufferSubData(vb, 0, sim.numberParticles, bufferCopy); 
  glUnmapBuffer(GL_ARRAY_BUFFER);

//  Model = glm::rotate(Model, 0.2f, glm::vec3(0,1,0));
  return 1;
}


int AhoyEngine::render(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
  glUseProgram(ShaderManager::get_program("basic"));

  glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
  glm::mat4 View = glm::lookAt(
      camPos, // 
      glm::vec3(0,0,0), // and looks at the origin
      glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
  );

  glm::mat4 mvp  = Projection * View * Model; // R

  glUniformMatrix4fv(mvp_id, 1, GL_FALSE, &mvp[0][0]);
   
  // 1rst attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vb);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
 
  glDrawArrays(GL_POINTS, 0, sim.numberParticles * 3); 
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
  //glDrawElements(
  //    GL_TRIANGLES,             
  //    boat_indices.size(),    
  //    GL_UNSIGNED_INT,          
  //    (void*)0                  
  //); 

  return 1;
}

int main(){
  AhoyEngine ahoy = AhoyEngine(640, 480, "Ahoy World!", 20);
  ahoy.run();
}

