#pragma once
#include "libs.h"
#include "transformable.h"
#include <glm/glm.hpp>
#include <vector>

using namespace std;
using namespace glm;

/*

  Camera abstraction for viewpoint modification.

 */
class Camera : public Transformable{

  private:
    mat4 projection;

  public:
    Camera();
    Camera(const mat4&);
    Camera(const mat4&, const mat4&);

    vec3 up();
    vec3 right();
    vec3 forward();

    mat4 get_vp();
    mat4 get_view();
    mat4 get_projection();
};
