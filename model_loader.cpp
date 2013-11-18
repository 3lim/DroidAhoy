#include "headers/model_loader.h"
#include <SOIL/SOIL.h>
#include <iostream>

/*
 *  Uses SOIL to load textures into memory.
 */

using namespace std;

ModelGroup ModelLoader::mdl = ModelGroup();

ModelGroup ModelLoader::get_last_model(){
  return mdl;
}

GLuint ModelLoader::load_texture(const string & path, const GLenum active_texture){
  glActiveTexture(active_texture);
  GLuint texture_id = SOIL_load_OGL_texture
    (
     path.c_str(),
     SOIL_LOAD_AUTO,
     SOIL_CREATE_NEW_ID,
     SOIL_FLAG_INVERT_Y
     );
  if(texture_id == 0){
    cerr << "SOIL loading error: '" << SOIL_last_result() << "' (" << path << ")" << endl;
    texture_id = -1;
  }
  cout << "Successfully loaded texture " << path << endl << endl;
  return texture_id;
}

vec3 ModelLoader::get_approx_center(ModelGroup & mdl){
  vec3 h(0,0,0), l(0,0,0), c(0,0,0);
  //Find l & h x,y,z coordinate
  cout << mdl.mdls.size() << endl;
  for(unsigned i=0; i<mdl.mdls.size(); i++){
    for(unsigned j=0; j<mdl.mdls[i].v.size(); j+=3){
     float x = mdl.mdls[i].v[j], y = mdl.mdls[i].v[j+1], z = mdl.mdls[i].v[j+2];
     if( x < l.x )
      l.x = x;
     if( y < l.y)
      l.y = y;
     if(z < l.z )
      l.z = z;
     if( x > h.x )
      h.x = x;
     if( y > h.y)
      h.y = y;
     if(z > h.z )
      h.z = z;
    }
  }
  
  c.x = h.x - ((h.x-l.x)*0.5f);
  c.y = h.y - ((h.y-l.y)*0.5f);
  c.z = h.z - ((h.z-l.z)*0.5f);
  return c; 
}
