#include "headers/model.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace std;

vec3 Model::up(){
  return _up * inverse(_ori);
}

vec3 Model::forward(){
  return _forward * inverse(_ori);
}

vec3 Model::right(){
  return _right * inverse(_ori);
}

Model::Model(){
  ready_for_draw = has_program= has_v = has_uv = has_n = has_indices = has_tex = false;
  has_v = false;
  has_uv = false;
  has_n = false;
  has_indices = false;
}

Model::Model(const Model& that) : 
  Transformable(that),
  vb(that.vb),
  ib(that.ib),
  uvb(that.uvb),
  nb(that.nb),
  tex(that.tex),
  mvp_id(that.mvp_id),
  program_id(that.program_id),
  ready_for_draw(that.ready_for_draw),
  has_v(that.has_v),
  has_indices(that.has_indices),
  has_uv(that.has_uv),
  has_n(that.has_n),
  has_tex(that.has_tex),
  has_program(that.has_program),
  v(that.v),
  n(that.n),
  uv(that.uv),
  indices(that.indices),
  tex_params(that.tex_params)
{}

Model::Model(const vector<float>& _v, const vector<float>& _n, const vector<float>& _uv, const vector<unsigned>& _indices, const TextureParams& _tex){
  v = _v;
  uv = _uv;
  indices = _indices;
  tex_params = _tex;
  
  ready_for_draw = has_v = has_uv = has_n = has_indices =  true;
  has_program = false;
  init();
}

/*

   Allocates OpenGL buffers for drawing to the framebuffer. For the texture
   it is assumed that is has been loaded elsewhere.

   Also assumed that the render program for the model includes a MVP uniform.

 */
bool Model::init(){
  mvp_id = glGetUniformLocation(program_id, "MVP");

  if(has_v){   
    glGenBuffers(1, &vb);
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*v.size(), v.data(), GL_STATIC_DRAW);
  }

  if(has_indices){
    glGenBuffers(1, &ib);
    glBindBuffer(GL_ARRAY_BUFFER, ib);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*indices.size(), indices.data(), GL_STATIC_DRAW);
  }

  if(has_uv){
    glGenBuffers(1, &uvb);
    glBindBuffer(GL_ARRAY_BUFFER, uvb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*uv.size(), uv.data(), GL_STATIC_DRAW);
  }

  if(has_n){
    glGenBuffers(1, &nb);
    glBindBuffer(GL_ARRAY_BUFFER, nb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*n.size(), n.data(), GL_STATIC_DRAW);
  }

  bool ready = false;
  ready = has_program && has_v && has_indices;
  if(has_uv)
    ready = ready && has_n && has_tex;
  
  return ready_for_draw = ready;
}

/*
   Mutators and accessors for model properties (vertices, normals, uvs, indices & texture)
 */
void Model::set_vertices(const vector<float>& _v){
  v = _v;
  has_v = true;
}

/*
   Accessor for vertices
 */
const vector<float>& Model::get_vertices(){
  return v;
}

void Model::set_normals(const vector<float>& _n){
  n = _n;
  has_n = true;
}

const vector<float>& Model::get_normals(){
  return n;
}

void Model::set_indices(const vector<unsigned>& _indices){
  indices = _indices;
  has_indices = true;
}

const vector<unsigned>& Model::get_indices(){
  return indices;
}

void Model::set_uvs(const vector<float>& _uv){
  uv = _uv;
  has_uv = true;
}

const vector<float>& Model::get_uvs(){
  return uv;  
}

void Model::set_texture(unsigned _tex_id){
  tex = _tex_id;
  has_tex = true;
}

void Model::set_texture_params(const TextureParams& _tex_params){
  tex_params = _tex_params;
}

/*

    Draws the model onto the framebuffer. Assumes the following layout for the shader:
    
      1: vertex
      2: normal
      3: uv 

*/
void Model::draw(const mat4 &vp){
  if(ready_for_draw){
    glUseProgram(program_id);

    //Compute mvp for this model
    mat4 mvp  = vp * get_transformation(); // R

    glUniformMatrix4fv(mvp_id, 1, GL_FALSE, &mvp[0][0]);
    //Vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    //normals
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, nb);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(void*)0);

    //texture
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, uvb);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,0,(void*)0);

    //glDrawArrays(GL_TRIANGLES, 0, boat_vertices.size()); 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
    glDrawElements(
        GL_TRIANGLES,             
        indices.size(),    
        GL_UNSIGNED_INT,          
        (void*)0                  
    );
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
  } else{
    cerr << "Model not ready to be drawn." << endl;
    cerr << "\tVertices? " << has_v <<endl;
    cerr << "\tIndices? " << has_indices <<endl;
    cerr << "\tUVs? " << has_uv <<endl;
    if(has_uv)
      cerr << "\tNormals? " << has_n <<endl;
      cerr << "\tLoaded texture? " << has_tex <<endl;
    cerr << "\tLoaded program? " << has_program <<endl;
  }
}

void Model::set_program(unsigned pid){
  program_id = pid;
  has_program = true;
}
