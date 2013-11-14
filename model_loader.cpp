#include "headers/model_loader.h"

mdl_group ModelLoader::mdl = mdl_group();

mdl_group ModelLoader::get_last_model(){
  return mdl;
}
