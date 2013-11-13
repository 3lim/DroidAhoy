#include "libs.h"
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

struct mdl_data{
  vec3 root;
  vector<vec3> v;
  vector<int> v_indices;
  vector<vec3> n;
  vector<int> n_indices;
  vector<vec3> uv;
  vector<int> uv_indices;
  string t_path;
};

class ModelLoader{
  private:

  public:
  
  protected:

};
