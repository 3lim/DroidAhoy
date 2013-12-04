#pragma once
#include "libs.h"
#include "transformable.h"
#include "texture_params.h"
#include <glm/glm.hpp>
#include <vector>

using namespace std;
using namespace glm;

/*

  Struct inspired by tiny_obj_loader

  Redefined as class to include object specific
  coordinate axles (_up, _right, _up x _right) and 
  more easily interpreted mutators.

 */
class Model : public Transformable{

  private:
    //Buffer ids:
    GLuint vb, ib, uvb, nb, tex;
  
    //Uniform ids, program id
    GLuint mvp_id, program_id;

    bool ready_for_draw, has_v, has_indices, has_uv, has_n, has_tex, has_program;
    
    vector<float> v;
    vector<float> n;
    vector<float> uv;
    vector<unsigned> indices;
    
    TextureParams tex_params;

  public:
    Model();
    Model(const Model&);
    Model(const vector<float>&, const vector<float>&, const vector<float>&, const vector<unsigned>&, const TextureParams&);
    
    void set_program(unsigned);

    bool init();
    void draw(const mat4&);  

    void set_vertices(const vector<float>&);
    const vector<float>& get_vertices();
    
    void set_normals(const vector<float>&);
    const vector<float>& get_normals();
    
    void set_indices(const vector<unsigned>&);
    const vector<unsigned>& get_indices();
    
    void set_uvs(const vector<float>&);
    const vector<float>& get_uvs();
    
    void set_texture(unsigned);
    void set_texture_params(const TextureParams&);

};
