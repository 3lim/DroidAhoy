#include "headers/shader_manager.h"
#include <fstream>
#include <cstdio>

map<string, GLuint> ShaderManager::current_shaders = map<string, GLuint>();
map<string, GLuint> ShaderManager::programs = map<string, GLuint>();
map<string, GLenum> ShaderManager::shader_types = { make_pair("vertex", GL_VERTEX_SHADER),
                                                    make_pair("fragment", GL_FRAGMENT_SHADER),
                                                    make_pair("geometry", GL_GEOMETRY_SHADER) };

bool ShaderManager::compile_shader(const string code, const string path){
  char const * source = code.c_str();
  GLuint shader_id = current_shaders[path];
  glShaderSource(shader_id, 1, &source , NULL);
  glCompileShader(shader_id);

  // Check Vertex Shader
  GLint success = GL_FALSE;
  int log_length;
  
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
  glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_length);
  vector<char> error_msg(log_length);
  
  if(success == GL_FALSE){
    glGetShaderInfoLog(shader_id, log_length, NULL, &error_msg[0]);
    cerr << &error_msg[0] << endl;
  }else{
    cout << "Shader \"" << path << "\" was successfully compiled.\n";
  }
  return success == GL_TRUE;
}

/*
 *  Helper function to read, create shader and return the code.
 */
string ShaderManager::read_shader(const string path){
  string shader_code = "";
  string file_type = path.substr(path.find(".")+1, path.length());
  GLenum shader_type;
  bool valid_type = false;
  if(shader_types.count(file_type)){
    shader_type = shader_types[file_type];
    valid_type = true;
  }
  if(!valid_type){
    cout << "Error: Invalid shader type" << endl;    
    throw std::string("Error: Invalid shader type");    
  }
  cout << "Parsed as " << file_type << " shader." << endl; 
  current_shaders[path] = glCreateShader(shader_type);

  ifstream file_stream(path, ios::in);
  if(file_stream.is_open()){
    string l = "";
    while(getline(file_stream, l))
      shader_code += "\n" + l;
    file_stream.close();
  }
  return shader_code;
}

/*
 *  Link program with currently loaded shaders.
 */
GLuint ShaderManager::link_shaders(string p_name){
  auto shader = current_shaders.begin();
  GLuint pid = glCreateProgram();

  while(shader != current_shaders.end()){
    glAttachShader(pid, shader -> second);
    ++shader;
  } 
 
  glLinkProgram(pid);

  GLint success = GL_FALSE;
  int log_length;
  
  // Check the program
  glGetProgramiv(pid, GL_LINK_STATUS, &success);
  glGetProgramiv(pid, GL_INFO_LOG_LENGTH, &log_length);
  vector<char> error_msg(log_length);
  glGetProgramInfoLog(pid, log_length, NULL, &error_msg[0]);
  cout << &error_msg[0] << endl;

  shader = current_shaders.begin();
  while(shader != current_shaders.end()){
    glDeleteShader(shader -> second);
    ++shader;
  } 
  current_shaders.clear();
  programs[p_name] = pid;

  if(success == GL_TRUE){
    cout << "Successfully linked program \"" << p_name << "\"" << endl;
  }
  
  return pid; 
}

bool ShaderManager::load_shader(string s_path){
  cout << "Parsing shader :" << s_path << endl;
  string code = read_shader(s_path);
  cout << "Compiling shader :" << s_path << endl;
  return compile_shader(code, s_path);
}

int ShaderManager::load_shaders(string path){
  return -1;
}

bool ShaderManager::delete_shader(string name){
  return false;
}

int ShaderManager::delete_all_shaders(){
  return -1;
}

GLuint ShaderManager::get_shader_gluid(string s_name){
  return GLuint(1);
}

GLuint ShaderManager::get_program(string p_name){
  return GLuint(1);
}
