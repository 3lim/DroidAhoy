#include <vector>

using namespace std;

/*

  Structs inspired by tiny_obj_loader

 */
struct Model{
  vector<float> v;
  vector<float> n;
  vector<float> uv;
  vector<unsigned> indices;
};
