#include "libs.h"
#include "model.h"
#include "texture.h"
#include "model_group.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;


class ModelLoader{
  private:
    static ModelGroup mdl;
  public:
    static ModelGroup get_last_model();
    static ModelGroup load_model(const string&);
    static GLuint load_texture(const string&, const GLenum);
    static vec3 get_approx_center(ModelGroup&);
 
  protected:

};
