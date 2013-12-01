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
  coordinate axles (up, right, up x right) and 
  more easily interpreted mutators.

 */
class Model : public Transformable{

  private:
    //Buffer ids
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
    Model(vector<float>, vector<float>, vector<float>, vector<unsigned>, TextureParams);
    
    void set_program(unsigned);

    bool init();
    void draw(mat4);  

    void set_vertices(vector<float>);
    const vector<float>& get_vertices();
    
    void set_normals(vector<float>);
    const vector<float>& get_normals();
    
    void set_indices(vector<unsigned>);
    const vector<unsigned>& get_indices();
    
    void set_uvs(vector<float>);
    const vector<float>& get_uvs();
    
    void set_texture(unsigned);
    void set_texture_params(TextureParams);

};
