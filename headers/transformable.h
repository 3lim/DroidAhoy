#pragma once
#include "libs.h"
#include "texture_params.h"
#include <glm/glm.hpp>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace std;
using namespace glm;

/*

  Transofrmable

 */
class Transformable{

  protected:
    mat4 _pt;
    bool _t_invalid;
    bool _t_order_reverse;
    vec3 _pos;
    quat _ori;

    //Use cross to determine forward
    vec3 _up;
    vec3 _right;
    vec3 _forward;
    float _scale;

  public:
    Transformable();
    
    const mat4& get_transformation();
   
    /* Model relative directions */ 
    virtual vec3 up();
    virtual vec3 right();
    virtual vec3 forward();
  
    void rotate(float,float,float);
    void set_rotation(vec3);
    void set_rotation(float,float,float);
    void set_rotation(const quat&);
    const vec3 get_rotation();

    void translate(float,float,float);
    void set_position(float,float,float);
    void set_position(vec3);
    const vec3& get_position();
    
    void set_up(float,float,float);
    void set_up(vec3);
    const vec3& get_up();
    
    void set_right(float,float,float);
    void set_right(vec3);
    const vec3& get_right();
  
    void set_scale(float);
    float get_scale();

};
