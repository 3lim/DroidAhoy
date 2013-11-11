#include "headers/shader_manager.h"

static int ShaderManager::link_shaders(){
  return -1;
}

static int ShaderManager::load_shader(string filename, GLenum type){
  return -1;
}

static int ShaderManager::load_shaders(string foldername, vector<GLenum> types){
  return -1;
}

static bool ShaderManager::reload_shader(int shader_id){
  return false;
}

static int ShaderManager::reload_shaders(string folder_name){
  return -1;
}

static int ShaderManager::delete_shader(){
  return -1;
}

static int ShaderManager::delete_all_shaders(){
  return -1;
}

static GLuint ShaderManager::get_shader_gluid(int shader_id){
  return GLuint(1);
}

static GLuint ShaderManager::get_program(int program_id){
  return GLuint(1);
}
