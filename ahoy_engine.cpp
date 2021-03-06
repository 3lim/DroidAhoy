#include "headers/ahoy_engine.h"
#include "headers/shader_manager.h"
#include "headers/obj_loader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ctime>

AhoyEngine::~AhoyEngine(){}

mat4 cam_old(1.0f);

// Model* boat_model;

int AhoyEngine::init(){
  
  if (GLEW_OK != glewInit()) {
      std::cout << "GLEW failed!" << std::endl;
      exit(1);
  }
  using namespace std;
  //glfwSetKeyCallback(window, key_callback );
  ShaderManager::load_program("shaders/basic");
  
    
  //glEnable(GL_CULL_FACE);

  ShaderManager::load_program("shaders/basic");
  ShaderManager::load_program("shaders/boat");
  
  sim = new SPHSimulation("parameters.txt");
  ak = new AndroidController();
  // sim->addBoat();
  //Load boat
  GLuint boat_tex_id = OBJLoader::load_texture(string("models/textures/pirate_boat.tga"), GL_TEXTURE0);
  Model* boat_model = OBJLoader::load_model_pointer(string("models/pirate_boat.obj"));
  boat_model->set_texture(boat_tex_id);
  boat_model->set_program(ShaderManager::get_program("boat"));
  boat_model->init();
  // boat_model->set_scale(0.005);
  boat_model->set_scale(0.01f);
  boat_model->set_rotation(90,0,0);
  boat_model->set_position(0,0,10);
  sim->addBoat(*boat_model);
  
  glm::vec3 cam_pos = glm::vec3(-0.8f,-0.8f, 0.15f);
  glm::mat4 p = glm::perspective(45.0f, 4.0f / 3.0f, 0.00001f, 10000.0f);
  glm::mat4 cam_init = glm::lookAt(cam_pos, glm::vec3(-0.3f,0.0f,-0.5f), glm::vec3(0,0,1));
  
  //Initialize camera
  cam = Camera(p, cam_init);
  cam.translate(0,0,-1.0f);
  cam_old = cam_init;

  //Attach window to keyboard controller 
  kb = KeyboardController(window);
  return 1;
}

clock_t start = clock();
float s_time = glfwGetTime();

int AhoyEngine::update(){
  // float dt = (float) ((float) clock() - start)/CLOCKS_PER_SEC; 
  float dt = (glfwGetTime()-s_time);
  s_time = glfwGetTime();
  sim -> update(dt/10);
  // std::cout << dt << std::endl;
  ak -> apply_input(sim, dt);
  kb.apply_input(cam,dt);
  //kb.apply_input(cam_old,dt);
  return 1;
}


int AhoyEngine::render(){
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glClearDepth(1.0);          
  glDepthFunc(GL_LEQUAL);           
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
  glEnable(GL_BLEND);           
  glAlphaFunc(GL_GREATER,0.1);
  glEnable(GL_ALPHA_TEST);          
  glEnable(GL_TEXTURE_2D);           
  
  sim->draw(cam.get_vp());


  return 1;
}

int main(){
  //AhoyEngine ahoy = AhoyEngine(1000,600,"Droid A'hoy");
  AhoyEngine ahoy = AhoyEngine("Droid A'hoy");
  ahoy.run();
}

