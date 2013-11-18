#include <string>

using namespace std;

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
