#include "libs.h"
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

/*

  Structs inspired by tiny_obj_loader

 */
struct mdl_data{
  vector<float> v;
  vector<float> n;
  vector<float> uv;
  vector<unsigned> indices;
};

struct tex_data{
  float ambient[3];
  float diffuse[3];
  float specular[3];
  float transmittance[3];
  float emission[3];
  float shininess;
  float ior;                // index of refraction

  string ambient_texpath;
  string diffuse_texpath;
  string specular_texpath;
  string normal_texpath;
};

struct mdl_group{
  vector<mdl_data> mdls;
  vector<tex_data> texs;
};

class ModelLoader{
  private:
    static mdl_group mdl;
  public:
    static mdl_group get_last_model();
    static mdl_group load_model(const string&);
 
  protected:

};
