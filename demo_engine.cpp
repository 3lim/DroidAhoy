#include "headers/primitives.h"
#include "headers/demo_engine.h"
#include "headers/shader_manager.h"
#include "headers/obj_loader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

DemoEngine::~DemoEngine(){}

GLuint vb;
GLuint mvp_id;
GLuint boat_tex_id;
GLuint elementbuffer, uvbuffer, normalbuffer;
vector<float> boat_vertices = vector<float>();
vector<float> boat_uv = vector<float>();
vector<float> boat_normals = vector<float>();
vector<unsigned> boat_indices = vector<unsigned>();
vec3 boat_center;
glm::vec3 camPos(4000,2000,2000);

int DemoEngine::init(){
  using namespace std;
  //glfwSetKeyCallback(window, key_callback );

  glewInit();
  ShaderManager::load_program("shaders/basic");
  
  //Load boat
  string boat_path = "models/boat.obj";
  boat_tex_id = OBJLoader::load_texture(string("models/Texture/boat.jpg"), GL_TEXTURE0);
  ModelGroup boat = OBJLoader::load_model(string("models/boat.obj"));
  boat_center = OBJLoader::get_approx_center(boat);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_MULTISAMPLE);

	glGenBuffers(1, &vb);
	glBindBuffer(GL_ARRAY_BUFFER, vb);

  //Converting model vertices to one array buffer
  for(unsigned i=0; i < 1; i++){
    for(unsigned j=0; j<boat.mdls[i].v.size(); j++)
      boat_vertices.push_back(boat.mdls[i].v[j]);
    for(unsigned j=0; j<boat.mdls[i].indices.size(); j++) 
      boat_indices.push_back(boat.mdls[i].indices[j]);
    for(unsigned j=0; j<boat.mdls[i].uv.size(); j++) 
      boat_uv.push_back(boat.mdls[i].uv[j]);
    for(unsigned j=0; j<boat.mdls[i].n.size(); j++) 
      boat_normals.push_back(boat.mdls[i].n[j]);
  }
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * boat_vertices.size(), &boat_vertices[0], GL_STATIC_DRAW); 

  glGenBuffers(1, &elementbuffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * boat_indices.size(), &boat_indices[0], GL_STATIC_DRAW);
  
  mvp_id = glGetUniformLocation(ShaderManager::get_program("basic"), "MVP");

  /* Texture buffers */
  glGenBuffers(1, &uvbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float)*boat_uv.size(), boat_uv.data(), GL_STATIC_DRAW);
  glGenBuffers(1, &normalbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float)*boat_normals.size(), boat_normals.data(), GL_STATIC_DRAW);

  return 1;
}

glm::mat4 Model = glm::mat4(1.0f);  // Changes for each model !

int DemoEngine::update(){
  Model = glm::rotate(Model, 0.2f, glm::vec3(0,1,0));
  return 1;
}


int DemoEngine::render(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
  glUseProgram(ShaderManager::get_program("basic"));

  glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
  glm::mat4 View = glm::lookAt(
      camPos, // 
      boat_center, // and looks at the origin
      glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
  );

  glm::mat4 mvp        = Projection * View * Model; // R

  glUniformMatrix4fv(mvp_id, 1, GL_FALSE, &mvp[0][0]);
   
  //Vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vb);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  //normals
  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
  glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,0,(void*)0);
  
  //texture
  glEnableVertexAttribArray(3);
  glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
  glVertexAttribPointer(3,2,GL_FLOAT,GL_FALSE,0,(void*)0);

  //glDrawArrays(GL_TRIANGLES, 0, boat_vertices.size()); 
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
  glDrawElements(
      GL_TRIANGLES,             
      boat_indices.size(),    
      GL_UNSIGNED_INT,          
      (void*)0                  
  ); 

  return 1;
}

int main(){
  DemoEngine ahoy = DemoEngine(640, 480, "Demo World!");
  ahoy.run();
}

