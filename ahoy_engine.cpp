#include "headers/primitives.h"
#include "headers/ahoy_engine.h"
#include "headers/shader_manager.h"
#include "headers/obj_loader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ctime>

AhoyEngine::~AhoyEngine(){}

GLuint vb;
GLuint mvp_id;
GLuint boat_tex_id;
GLuint elementbuffer, uvbuffer, normalbuffer;
vector<float> boat_vertices = vector<float>();
vector<float> boat_uv = vector<float>();
vector<float> boat_normals = vector<float>();
vector<unsigned> boat_indices = vector<unsigned>();
vec3 boat_center;
Model boat;

int AhoyEngine::init(){
  using namespace std;
  //glfwSetKeyCallback(window, key_callback );
  
  glewInit();
  
  //glEnable(GL_CULL_FACE);

  ShaderManager::load_program("shaders/basic");
 
  //Load boat
  boat_tex_id = OBJLoader::load_texture(string("models/textures/pirate_boat.tga"), GL_TEXTURE0);
  boat = OBJLoader::load_model(string("models/pirate_boat.obj"));
  boat.set_texture(boat_tex_id);
  boat.set_program(ShaderManager::get_program("basic"));
  boat.init();
  boat_center = OBJLoader::get_approx_center(boat);
  
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
  float dt = (float) ((float) clock() - start)/CLOCKS_PER_SEC; 
  boat.add_rotation(10.0f*dt, 10.0f*dt, 0.0f);
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

  return 1;
}

int main(){
  AhoyEngine ahoy = AhoyEngine(640, 480, "Demo World!");
  ahoy.run();
}

