#include "headers/libs.h"
#include "headers/ahoy_engine.h"
#include "headers/shader_manager.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

AhoyEngine::~AhoyEngine(){}

static const GLfloat cube[] = {
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};

GLuint vaid;
GLuint vb;
GLuint mvp_id;

int AhoyEngine::init(){
  if (GLEW_OK != glewInit())
	{
	    std::cout << "GLEW failed!" << std::endl;
	    exit(1);
	}
  ShaderManager::load_program("shaders/basic");
 // ShaderManager::load_shader("shaders/mvp.vertex");
 // ShaderManager::load_shader("shaders/light.fragment");
 // ShaderManager::link_shaders("basic");

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_MULTISAMPLE);

  
	glGenVertexArrays(1, &vaid);
	glBindVertexArray(vaid);
	glGenBuffers(1, &vb);
	glBindBuffer(GL_ARRAY_BUFFER, vb);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW); 

  mvp_id = glGetUniformLocation(ShaderManager::get_program("basic"), "MVP");
  
  return 1;
}

int AhoyEngine::update(){
  sim.update(timeStep);
  return 1;
}

int AhoyEngine::render(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
  // glUseProgram(ShaderManager::get_program("basic"));

  glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
  glm::mat4 View       = glm::lookAt(
      glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
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
  
  glDrawArrays(GL_TRIANGLES, 0, sizeof(cube));
  return 1;
}

int main(){
  AhoyEngine ahoy = AhoyEngine(640, 480, "Ahoy World!");
  ahoy.run();
}
