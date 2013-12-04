#pragma once
#include "libs.h"
#include "texture_params.h"
#include <glm/glm.hpp>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

/*

  Transofrmable

 */
class Transformable{

  protected:
    mat4 pt;
    bool t_invalid;
    bool t_order_reverse;
    vec3 rot;
    vec3 pos;

    //Use cross to determine forward
    vec3 up;
    vec3 right;
    vec3 forward;
    float _scale;

  public:
    Transformable();
    
    const mat4& get_transformation();
    
    void add_rotation(float,float,float);
    void set_rotation(const vec3&);
    void set_rotation(float,float,float);
    const vec3& get_rotation();

    void add_position(float,float,float);
    void set_position(float,float,float);
    void set_position(const vec3&);
    const vec3& get_position();
    
    void set_up(float,float,float);
    void set_up(const vec3&);
    const vec3& get_up();
    
    void set_right(float,float,float);
    void set_right(const vec3&);
    const vec3& get_right();
  
    void set_scale(float);
    float get_scale();

};
