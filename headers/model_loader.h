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
struct Model{
  vector<float> v;
  vector<float> n;
  vector<float> uv;
  vector<unsigned> indices;
};

struct Texture{
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

struct ModelGroup{
  vector<Model> mdls;
  vector<Texture> texs;
};

class ModelLoader{
  private:
    static ModelGroup mdl;
  public:
    static ModelGroup get_last_model();
    static ModelGroup load_model(const string&);
 
  protected:

};
