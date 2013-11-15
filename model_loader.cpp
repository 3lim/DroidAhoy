#include "headers/model_loader.h"

ModelGroup ModelLoader::mdl = ModelGroup();

ModelGroup ModelLoader::get_last_model(){
  return mdl;
}
