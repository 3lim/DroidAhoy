#include "headers/libs.h"
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
vector<float> teapot_vertices = vector<float>();
vector<unsigned> teapot_indices = vector<unsigned>();

int AhoyEngine::init(){
  using namespace std;

  glewInit();
  ShaderManager::load_program("shaders/basic");
 // ShaderManager::load_shader("shaders/mvp.vertex");
 // ShaderManager::load_shader("shaders/light.fragment");
 // ShaderManager::link_shaders("basic");
  
  //Load teapot
  string teapot_path = "models/teapot.obj";
  ModelGroup teapot = OBJLoader::load_model(teapot_path);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_MULTISAMPLE);

	glGenBuffers(1, &vb);
	glBindBuffer(GL_ARRAY_BUFFER, vb);

  //Converting model vertices to one array buffer
  for(unsigned i=0; i < teapot.mdls.size(); i++){
    for(unsigned j=0; j<teapot.mdls[i].v.size(); j++)
      teapot_vertices.push_back(teapot.mdls[i].v[j]);
    for(unsigned j=0; j<teapot.mdls[i].indices.size(); j++) 
      teapot_indices.push_back(teapot.mdls[i].indices[j]);
  }
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * teapot_vertices.size(), &teapot_vertices[0], GL_STATIC_DRAW); 

  glGenBuffers(1, &elementbuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * teapot_indices.size(), &teapot_indices[0], GL_STATIC_DRAW);
  
  mvp_id = glGetUniformLocation(ShaderManager::get_program("basic"), "MVP");

  return 1;
}

int AhoyEngine::update(){
  return 1;
}

int AhoyEngine::render(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
  glUseProgram(ShaderManager::get_program("basic"));

  glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
  glm::mat4 View       = glm::lookAt(
      glm::vec3(14,8,8), // Camera is at (4,3,3), in World Space
      glm::vec3(0,0,0), // and looks at the origin
      glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
  );

  glm::mat4 Model      = glm::mat4(1.0f);  // Changes for each model !
  glm::mat4 mvp        = Projection * View * Model; // R

  glUniformMatrix4fv(mvp_id, 1, GL_FALSE, &mvp[0][0]);
   
  // 1rst attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vb);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
 
  //glDrawArrays(GL_TRIANGLES, 0, teapot_vertices.size()); 
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
  glDrawElements(
      GL_TRIANGLES,             
      teapot_indices.size(),    
      GL_UNSIGNED_INT,          
      (void*)0                  
  ); 

  return 1;
}

int main(){
  AhoyEngine ahoy = AhoyEngine(640, 480, "Ahoy World!");
  ahoy.run();
}
