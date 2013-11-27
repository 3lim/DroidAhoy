#include "headers/model.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace std;

Model::Model(){
  ready_for_draw = has_program= has_v = has_uv = has_n = has_indices = has_tex = false;
  has_v = false;
  has_uv = false;
  has_n = false;
  has_indices = false;
  t_invalid = false;
  pt = mat4(1.0f);
  up = vec3(0.0f, 1.0f, 0.0f);
  right = vec3(1.0f, 0.0f, 0.0f);
}

Model::Model(vector<float> _v, vector<float> _n, vector<float> _uv, vector<unsigned> _indices, TextureParams _tex){
  v = _v;
  uv = _uv;
  indices = _indices;
  tex_params = _tex;
  t_invalid = false;
  pt = mat4(1.0f);
  up = vec3(0.0f, 1.0f, 0.0f);
  right = vec3(1.0f, 0.0f, 0.0f);

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
void Model::set_vertices(vector<float> _v){
  v = _v;
  has_v = true;
}

/*
   Accessor for vertices
 */
const vector<float>& Model::get_vertices(){
  return v;
}

void Model::set_normals(vector<float> _n){
  n = _n;
  has_n = true;
}

const vector<float>& Model::get_normals(){
  return n;
}

void Model::set_indices(vector<unsigned> _indices){
  indices = _indices;
  has_indices = true;
}

const vector<unsigned>& Model::get_indices(){
  return indices;
}

void Model::set_uvs(vector<float> _uv){
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

void Model::set_texture_params(TextureParams _tex_params){
  tex_params = _tex_params;
}

/*

   Draws the model onto the framebuffer. Assumes the following layout for the shader:

1: vertex
2: normal
3: uv 


 */
void Model::draw(mat4 vp){
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
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, nb);
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,0,(void*)0);

    //texture
    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, uvb);
    glVertexAttribPointer(3,2,GL_FLOAT,GL_FALSE,0,(void*)0);

    //glDrawArrays(GL_TRIANGLES, 0, boat_vertices.size()); 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
    glDrawElements(
        GL_TRIANGLES,             
        indices.size(),    
        GL_UNSIGNED_INT,          
        (void*)0                  
    );
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

mat4 Model::get_transformation(){
  if(t_invalid){
    //Form rotation
    mat4 rotX(1.0f), rotY(1.0f), rotZ(1.0f), rota;
    rotX = rotate(rotX, rot.x, right);
    rotY = rotate(rotY, rot.y, up);
    rotZ = rotate(rotZ, rot.z, cross(right,up));
    rota = rotZ * rotY * rotX;

    //Form translation
    mat4 transX(1.0f), transY(1.0f), transZ(1.0f), transa;
    transX = translate(transX, pos.x*right);
    transY = translate(transY, pos.y*up);
    transZ = translate(transZ, pos.z*cross(right,up));
    transa = transZ * transY * transZ;

    //Total transformation
    return pt = rota * transa;
  }
  else{
    return pt;
  }
}

void Model::set_rotation(float ax, float ay, float az){
  rot.x = ax;
  rot.y = ay;
  rot.z = az;
}

void Model::set_rotation(vec3 r){
  t_invalid = true;
  rot = r;
}

void Model::add_rotation(float ax, float ay, float az){
  t_invalid = true;
  rot.x += ax;
  rot.y += ay;
  rot.z += az;
}

const vec3& Model::get_rotation(){
  return rot;
}

void Model::set_position(float x, float y, float z){
  t_invalid = true;
  pos.x = x;
  pos.y = y;
  pos.z = z;
}

void Model::set_position(vec3 p){
  t_invalid = true;
  pos = p;
}

void Model::add_position(float x, float y, float z){
  t_invalid = true;
  pos.x += x;
  pos.y += y;
  pos.z += z;
}

const vec3& Model::get_position(){
  return pos;
}


/*

   The up and right are used to determine around which axises to perform
   rotations and translations.

 */
void Model::set_up(float x, float y, float z){
  t_invalid = true;
  up = vec3(x,y,z);
}

void Model::set_up(vec3 _up){
  t_invalid = true;
  up = _up;
}

const vec3& Model::get_up(){
  return up;
}

void Model::set_right(vec3 _right){
  t_invalid = true;
  right = _right;
}

void Model::set_right(float x, float y, float z){
  t_invalid = true;
  right = vec3(x,y,z);
}

const vec3& Model::get_right(){
  return right;
}

void Model::set_program(unsigned pid){
  program_id = pid;
  has_program = true;
}
