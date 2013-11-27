#include "libs.h"
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
class Model{

  private:
    mat4 pt;
    bool t_invalid;
  
    //Buffer ids
    GLuint vb, ib, uvb, nb, tex;
  
    //Uniform ids, program id
    GLuint mvp_id, program_id;

    bool ready_for_draw, has_v, has_indices, has_uv, has_n, has_tex, has_program;
    vec3 rot;
    vec3 pos;

    //Use cross to determine forward
    vec3 up;
    vec3 right;
    
    vector<float> v;
    vector<float> n;
    vector<float> uv;
    vector<unsigned> indices;
    
    TextureParams tex_params;

  public:
    Model();
    Model(vector<float>, vector<float>, vector<float>, vector<unsigned>, TextureParams);
    
    mat4 get_transformation();
    
    void set_program(unsigned);

    void add_rotation(float,float,float);
    void set_rotation(vec3);
    void set_rotation(float,float,float);
    const vec3& get_rotation();

    void set_position(float,float,float);
    void set_position(vec3);
    void add_position(float,float,float);
    const vec3& get_position();
    
    void set_up(float,float,float);
    void set_up(vec3);
    const vec3& get_up();
    
    void set_right(float,float,float);
    void set_right(vec3);
    const vec3& get_right();

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
