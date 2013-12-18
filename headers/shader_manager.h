#pragma once
#include "libs.h"
#include <string>
#include <vector>
#include <map>
#include <iostream>

// Shader types (GLenum):
//    GL_COMPUTE_SHADER,
//    GL_VERTEX_SHADER,
//    GL_TESS_CONTROL_SHADER,
//    GL_TESS_EVALUATION_SHADER,
//    GL_GEOMETRY_SHADER,
// or GL_FRAGMENT_SHADER.
using namespace std;
static map<string, GLenum> shader_types = { make_pair("vertex", GL_VERTEX_SHADER),
                                            make_pair("fragment", GL_FRAGMENT_SHADER),
                                            make_pair("geometry", GL_GEOMETRY_SHADER) };

/*
 *  Class to read, load and compile shader programs. The files must use a file-extension
 *  matching one of the strings in shader_types in order to properly parse the shader.
 *  Only shaders with no warnings or errors will be accepted.
 */
class ShaderManager{
  private:
    ShaderManager() = delete;
    ~ShaderManager() = delete;
    ShaderManager& operator=(ShaderManager&) = delete;
    static map<string, GLuint> programs; 
    static map<string, GLuint> current_shaders;
    static bool compile_shader(const string, const string);
    static string read_shader(const string);
    static map<string, GLenum> shader_types;
                                           
  public:
    static GLuint link_shaders(string program_name);
    static bool load_shader(string file_path);
    static int load_program(string folder_path); 
    static bool unload_shader(string s_name);
    static int reset_shaders();
    static GLuint get_program(string p_name);
    static bool delete_program(string p_name);
  
  protected:

};
