#pragma once
#include <freeglut>
#include <string>
//GL_COMPUTE_SHADER,
//GL_VERTEX_SHADER,
//GL_TESS_CONTROL_SHADER,
//GL_TESS_EVALUATION_SHADER,
//GL_GEOMETRY_SHADER,
//or GL_FRAGMENT_SHADER.

class ShaderManager{

  private:
    ShaderManager() = delete;
    ~ShaderManager() = delete;
    ShaderManager& operator=() = delete;
    vector<string> shader_files;
    vector<GLuint> shader_glids, shader_programs; 
    vector<GLuint> current_shaders;

  public:
    static int link_shaders();
    static int load_shader(string, GLenum);
    static int load_shaders(string, 
    static bool reload_shader(int);
    static int reload_shaders(string folder_name);
    static int delete_shader();
    static int delete_all_shaders();
    static GLuint get_shader_gluid(int shader_id);
    static GLuint get_program(int program_id=0);
  protected:

}
