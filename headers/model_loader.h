#include "libs.h"
#include "model.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

/*
 *  Interface to provide different file-format loading.
 *  The implemented loader must provide a Model instance
 *  as a return value.
 */
class ModelLoader{
  private:
    static Model mdl;
  public:
    static Model get_last_model();
    static Model load_model(const string&);
    static GLuint load_texture(const string&, const GLenum);
    static vec3 get_approx_center(Model&);
 
  protected:

};
