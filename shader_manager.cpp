#include "headers/shader_manager.h"
#include <fstream>
#include <cstdio>
#include <boost/filesystem/operations.hpp> 
#include <boost/filesystem/fstream.hpp>    

/*
###########################################################################
###########################################################################


  Shader Manager

  requires:
    boost
    glfw3
    glm
    
  installing boost (ubuntu):
    sudo a_pt-get install libboost-all-dev

  installing glfw3 (ubuntu):
    see README.md

  installing glm (ubuntu):
    included in ubuntu, otherwise:
    sudo a_pt-get install libglm-dev
    

###########################################################################
###########################################################################
*/
map<string, GLuint> ShaderManager::current_shaders = map<string, GLuint>();
map<string, GLuint> ShaderManager::programs = map<string, GLuint>();
map<string, GLenum> ShaderManager::shader_types = { make_pair("vertex", GL_VERTEX_SHADER),
                                                    make_pair("fragment", GL_FRAGMENT_SHADER),
                                                    make_pair("geometry", GL_GEOMETRY_SHADER) };

/*
 *  Compiles the given shader code. Requires file path due to mapping between
 *  string path and GLuint, i.e. shader id.
 */
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
    cout << "Shader \"" << path << "\" was successfully compiled." << endl << endl;
  }
  return success == GL_TRUE;
}

/*
 *  Helper function to read, create shader and return the code.
 *  Assumes the following of the input file:
 *    * file extension designates shader type, currently supports:
 *      - .vertex
 *      - .geometry
 *      - .fragment
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
    cout << "Error: Invalid shader type." << endl;    
    throw std::string("Error: Invalid shader type.");    
  }
  cout << "Parsed as " << file_type << " shader." << endl << endl; 

  //Only create new shader if nonexistant  
  if(!current_shaders.count(path))
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

  reset_shaders();
  
  programs[p_name] = pid;

  if(success == GL_TRUE){
    cout << "Successfully linked program \"" << p_name << "\"" << endl << endl;
  }
  
  return pid; 
}

/*
 *  Loads and compiles shader file found by given path.
 */
bool ShaderManager::load_shader(string s_path){
  cout << "Parsing shader: "<< s_path << endl;
  string code = read_shader(s_path);
  cout << "Compiling shader: "<< s_path << endl;
  return compile_shader(code, s_path);
}

/*
 *  Loads an entire folder as a program. Assumes path
 *  leads to folder containing shaders.
 *
 */
using namespace boost::filesystem;
int ShaderManager::load_program(string p){
  int s_count=0;


  if( is_directory(p) ){
    //Determine program name
    string f_name;
    f_name = p.substr(p.find_last_of("/")+1, p.length()-1);

    //Return if program already exists
    if( programs[f_name] ) return programs[f_name];

    //Read and compile shaders in folder
    auto file = directory_iterator(p);
    string s_name;
    while( file != directory_iterator() ){
      s_name = (*file).path().string();
      load_shader( s_name );
      ++file;
      ++s_count;
    }
   
    
    //Link shaders into program
    link_shaders(f_name);
    return s_count;

  } else{
    cerr << "Error: Not a folder" << endl;
    return -1;
  }    
  
}

/*
 *  Removes shader from current pipeline.
 */
bool ShaderManager::unload_shader(string name){
  if(current_shaders.count(name)){
    glDeleteShader(current_shaders[name]);
    current_shaders.erase(name);
  }
  return true; 
}

/*
 *  Removes all currently loaded shaders from pipeline.
 */

int ShaderManager::reset_shaders(){
  int count = 0;
  auto shader = current_shaders.begin();
  while(shader != current_shaders.end()){
    glDeleteShader(shader -> second);
    ++shader;
    ++count;
  } 
  current_shaders.clear(); 
  return count;
}

/*
 *  Get the program id from the name given during
 *  compilation.
 */
GLuint ShaderManager::get_program(string p_name){
  return programs[p_name];
}

/*
 *  Deletes a linked program from memory.
 */
bool ShaderManager::delete_program(string p_name){
  if(programs.count(p_name)){
    GLuint pid = programs[p_name];
    GLint status = GL_FALSE;
    glDeleteProgram(programs[p_name]);
    programs.erase(p_name);
    
    glGetProgramiv(pid, GL_DELETE_STATUS, &status);
    if(status == GL_FALSE)
      return false;
    
  }
  return true;
}
