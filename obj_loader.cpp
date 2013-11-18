#include "headers/obj_loader.h"
#include "libs/tinyobj/tiny_obj_loader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits.h>

/*
    
    Uses tiny_obj_loader to parse .obj file.

*/
using namespace std;
using namespace glm;


ModelGroup OBJLoader::load_model(const string & filepath) {
  ModelGroup obj_data;
  obj_data.mdls = vector<Model>();
  obj_data.texs = vector<Texture>();

  cout << "Loading " << filepath << endl;

  //tiny_obj_loader parsing
  vector<tinyobj::shape_t> shapes;
  string base_path = filepath.substr(0, filepath.find_last_of("/")+1);
  string filename  = filepath.substr(filepath.find_last_of("/")+1, filepath.length());
  string err = tinyobj::LoadObj(shapes, filepath.c_str(), base_path.c_str());

  if (!err.empty()) {
    cerr << err << endl;
    return obj_data;
  }
  cout << "Successfully loaded " << filename << endl;
  cout << "# parts: " << shapes.size()  << endl;
  for(unsigned i=0; i<shapes.size(); i++)
    cout << "part #" << i << " vertices count: " << shapes[i].mesh.positions.size()  << endl
         <<  " material name: " << shapes[i].material.name << endl << endl;
  
  cout << endl;
  //Convert to Engine's internal mdl format
  auto it = shapes.begin();
  Model curr_mdl;
  Texture curr_tex;
   
  while(it != shapes.end()){
    curr_mdl.v = (*it).mesh.positions;
    curr_mdl.n = (*it).mesh.normals;
    curr_mdl.uv = (*it).mesh.texcoords;
    curr_mdl.indices = (*it).mesh.indices;

    for(int i=0; i<3; i++){
      curr_tex.ambient[i] = (*it).material.ambient[i];
      curr_tex.diffuse[i] = (*it).material.diffuse[i];
      curr_tex.specular[i] = (*it).material.specular[i];
      curr_tex.transmittance[i] = (*it).material.transmittance[i];
      curr_tex.emission[i] = (*it).material.emission[i];
    }
    curr_tex.shininess = (*it).material.shininess;
    curr_tex.ior = (*it).material.ior;
    curr_tex.ambient_texpath = (*it).material.ambient_texname;
    curr_tex.diffuse_texpath = (*it).material.diffuse_texname;
    curr_tex.specular_texpath = (*it).material.specular_texname;
    curr_tex.normal_texpath = (*it).material.normal_texname;
    
    obj_data.mdls.push_back(curr_mdl);
    obj_data.texs.push_back(curr_tex);
    ++it;   
  }

  return obj_data;
}	
