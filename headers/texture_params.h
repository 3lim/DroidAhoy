#pragma once
#include <string>

using namespace std;

/*
 *  Texture paramaters derived from tiny_obj. Assumption
 *  is that the .obj model has extra parameters for different
 *  faces, which are not currently supported but could be for
 *  future development.
 */
struct TextureParams{
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
