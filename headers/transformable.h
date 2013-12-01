#include "libs.h"
#include "texture_params.h"
#include <glm/glm.hpp>
#include <vector>

using namespace std;
using namespace glm;

/*

  Transofrmable

 */
class Transformable{

  private:
    mat4 pt;
    bool t_invalid;
    vec3 rot;
    vec3 pos;

    //Use cross to determine forward
    vec3 up;
    vec3 right;

  public:
    Transformable();
    
    mat4 get_transformation();
    
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
};
