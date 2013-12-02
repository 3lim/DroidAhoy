#include "headers/primitives.h"
#include "headers/demo_engine.h"
#include "headers/shader_manager.h"
#include "headers/obj_loader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ctime>

DemoEngine::~DemoEngine(){}

GLuint vb;
GLuint mvp_id;
GLuint boat_tex_id;
vec3 boat_center;
Model boat;
mat4 cam_old(1.0f);

int DemoEngine::init(){
  using namespace std;
  //glfwSetKeyCallback(window, key_callback );
  
  glewInit();
  
  //glEnable(GL_CULL_FACE);

  ShaderManager::load_program("shaders/basic");
 
  //Load boat
  GLuint boat_tex_id = OBJLoader::load_texture(string("models/textures/pirate_boat.tga"), GL_TEXTURE0);
  boat = OBJLoader::load_model(string("models/pirate_boat.obj"));
  boat.set_texture(boat_tex_id);
  boat.set_program(ShaderManager::get_program("basic"));
  boat.init();
  boat_center = OBJLoader::get_approx_center(boat);
  boat.set_scale(0.05f);
  boat.set_position(0,0,0);
  boat.set_rotation(90,0,0);
   
  glm::mat4 p = glm::perspective(45.0f, 4.0f / 3.0f, 0.00001f, 10000.0f);
  glm::mat4 cam_init = glm::lookAt(glm::vec3(15,5,5),boat_center, glm::vec3(0,1,0));

  //Initialize camera
  cam = Camera(p, cam_init);
  cam_old = cam_init;

  //Attach window to keyboard controller 
  kb = KeyboardController(window);
  return 1;
}

clock_t start = clock();
int DemoEngine::update(){
  float dt = (float) ((float) clock() - start)/CLOCKS_PER_SEC; 
  boat.rotate(0, -5.0f *dt, 0.0f);
  kb.apply_input(cam,dt);
  start = clock();
  return 1;
}


int DemoEngine::render(){
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glClearDepth(1.0);          
  glDepthFunc(GL_LEQUAL);           
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
  glEnable(GL_BLEND);           
  glAlphaFunc(GL_GREATER,0.1);
  glEnable(GL_ALPHA_TEST);          
  glEnable(GL_TEXTURE_2D);           

  //glm::mat4 p = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
  boat.draw(cam.get_view());

  return 1;
}

int main(){
  DemoEngine ahoy = DemoEngine(640, 480, "Demo World!");
  ahoy.run();
}

