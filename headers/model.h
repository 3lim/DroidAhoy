#include "libs.h"
#include <vector>

using namespace std;

using namespace glm;

/*

  Structs inspired by tiny_obj_loader

 */
struct Model{
  mat4 m;
  vector<float> v;
  vector<float> n;
  vector<float> uv;
  vector<unsigned> indices;
};
