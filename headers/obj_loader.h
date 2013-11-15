#include "libs.h"
#include "model_loader.h"
#include "../libs/tinyobj/tiny_obj_loader.h"

class OBJLoader : ModelLoader{
  private:
  public:
    static ModelGroup load_model(const string&);
};
