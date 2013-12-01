#include "headers/primitives.h"
#include "headers/ahoy_engine.h"
#include "headers/shader_manager.h"
#include "headers/obj_loader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ctime>

AhoyEngine::~AhoyEngine(){}

GLuint boat_tex_id;
vec3 boat_center;
Model boat;

int AhoyEngine::init(){

  if (GLEW_OK != glewInit())
  {
      std::cout << "GLEW failed!" << std::endl;
      exit(1);
  }
  using namespace std;
  //glfwSetKeyCallback(window, key_callback );
  ShaderManager::load_program("shaders/basic");
  
  sim = new SPHSimulation("parameters.txt");
  //Load boat
  boat_tex_id = OBJLoader::load_texture(string("models/textures/pirate_boat.tga"), GL_TEXTURE0);
  boat = OBJLoader::load_model(string("models/pirate_boat.obj"));
  boat.set_texture(boat_tex_id);
  boat.set_program(ShaderManager::get_program("basic"));
  boat.init();
  boat_center = OBJLoader::get_approx_center(boat);
  boat.add_position(0,0,10);

  //Set camera initial position
  cam = glm::lookAt(
      glm::vec3(15,5,5), // 
      boat_center, // and looks at the origin
      glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
  );

  //Attach window to keyboard controller 
  kb = KeyboardController(window);  
  return 1;
}

clock_t start = clock();
int AhoyEngine::update(){
  if (sim)
    sim->update(timeStep/2);

  float dt = (float) ((float) clock() - start)/CLOCKS_PER_SEC; 
  boat.add_rotation(0.0f, 10.0f*dt, 0.0f);
  kb.apply_input(cam,dt);
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

  glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);

  glm::mat4 vp = Projection * cam;

  boat.draw(vp);
  if (sim)
    sim->draw(vp);

  return 1;
}

int main(){
  AhoyEngine ahoy = AhoyEngine(640, 480, "Ahoy World!");
  ahoy.run();
}

