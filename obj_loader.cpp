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


Model OBJLoader::load_model(const string & filepath) {
  Model obj_data;
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
  //cout << "# parts: " << shapes.size()  << endl;
  //for(unsigned i=0; i<shapes.size(); i++)
  //  cout << "part #" << i << " vertices count: " << shapes[i].mesh.positions.size()  << endl
  //       <<  " material name: " << shapes[i].material.name << endl << endl;
  
  cout << endl;
  //Convert to Engine's internal mdl format
  auto it = shapes.begin();
   
  //OBS: Only loads .obj with 1 group (!!!)
  //while(it != shapes.end()){
    obj_data.v = (*it).mesh.positions;
    obj_data.n = (*it).mesh.normals;
    obj_data.uv = (*it).mesh.texcoords;
    obj_data.indices = (*it).mesh.indices;

    for(int i=0; i<3; i++){
      obj_data.tex.ambient[i] = (*it).material.ambient[i];
      obj_data.tex.diffuse[i] = (*it).material.diffuse[i];
      obj_data.tex.specular[i] = (*it).material.specular[i];
      obj_data.tex.transmittance[i] = (*it).material.transmittance[i];
      obj_data.tex.emission[i] = (*it).material.emission[i];
    }
    obj_data.tex.shininess = (*it).material.shininess;
    obj_data.tex.ior = (*it).material.ior;
    obj_data.tex.ambient_texpath = (*it).material.ambient_texname;
    obj_data.tex.diffuse_texpath = (*it).material.diffuse_texname;
    obj_data.tex.specular_texpath = (*it).material.specular_texname;
    obj_data.tex.normal_texpath = (*it).material.normal_texname;
    
    //obj_data.mdls.push_back(obj_data);
    //obj_data.texs.push_back(obj_data.tex);
    //++it;   
  //}

  return obj_data;
}	
