#include "headers/primitives.h"
#include "headers/ahoy_engine.h"
#include "headers/shader_manager.h"
#include "headers/obj_loader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ctime>

AhoyEngine::~AhoyEngine(){}

mat4 cam_old(1.0f);

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
  //Load boat
  GLuint boat_tex_id = OBJLoader::load_texture(string("models/textures/pirate_boat.tga"), GL_TEXTURE0);
  Model* boat_model = OBJLoader::load_model_pointer(string("models/pirate_boat.obj"));
  boat_model->set_texture(boat_tex_id);
  boat_model->set_program(ShaderManager::get_program("boat"));
  boat_model->init();
  boat_model->set_scale(0.02);
  boat_model->set_rotation(90,0,0);
  sim->addBoat(*boat_model);

  const vec3& boat_center = OBJLoader::get_approx_center(*boat_model);
  

  glm::mat4 p = glm::perspective(45.0f, 4.0f / 3.0f, 0.00001f, 10000.0f);
  glm::mat4 cam_init = glm::lookAt(glm::vec3(1,1,1), glm::vec3(0,0,1), glm::vec3(0,0,1));

  //Initialize camera
  cam = Camera(p, cam_init);
  cam_old = cam_init;

  //Attach window to keyboard controller */
  kb = KeyboardController(window);
  return 1;
}

clock_t start = clock();
int AhoyEngine::update(){
  sim->update(timeStep/5);

  float dt = (float) ((float) clock() - start)/CLOCKS_PER_SEC; 
  //boat->add_rotation(0.0f, 10.0f*dt, 0.0f);
  kb.apply_input(cam_old,dt);
  start = clock();
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

  glm::mat4 p = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
  glm::mat4 vp = p*cam_old;

  //boat->draw(vp);
  sim->draw(vp);
  /*glPushMatrix();
  //glRotatef(-45,1,0,0);
  //glScalef(0.5,0.5,0.5);
  //glTranslatef(0,0,0);
  sim->render();
  glPopMatrix();*/

  return 1;
}

int main(){
  AhoyEngine ahoy = AhoyEngine(640, 480, "Demo World!");
  ahoy.run();
}

