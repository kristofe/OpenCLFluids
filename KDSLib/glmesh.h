#ifndef GLMESH_H
#define GLMESH_H
#include "glutil.h"
#include "glprogram.h"
#include "glm/glm.hpp"
#include "Vertex.h"
#include <vector>


namespace kdslib{

struct UniformInfo
{
public:
  UniformInfo(const std::string pname):name(pname),location(0){};

public:
  std::string name;
  int location;
};

//TODO: Maybe templatize this class so it can have different kinds of vertices?
class GLMesh
{
//Methods
public:
  GLMesh(){};

  int GetVertexListSizeInBytes();
  int GetIndexListSizeInBytes();

private:
  DISALLOW_COPY_AND_ASSIGN(GLMesh);

//Data Members
protected:
  GLint _vao;
  GLint _vbo;

  std::vector<Vector3> _positions;
  std::vector<Vector3> _normals;
  std::vector<Vertex>  _verts;
  std::vector<GLuint> _indices;
  std::vector<VertexAttributeInfo> _attributes;
  std::vector<UniformInfo> _uniforms;


};


int GetVertexListSizeInBytes()
{
  return _positions.size()*sizeof(Vertex);
}

int GetIndexListSizeInBytes()
{
  return _indices.size()*sizeof(GLuint);
}

void CreateBuffers(GLProgram *program)
{
  /*
  TODO:This doesn't belong here.  We need a display object, renderer
  and material classes
  */



  // make and bind the VAO
  glGenVertexArrays(1, &_vao);
  glBindVertexArray(_vao);

  // make and bind the VBO
  glGenBuffers(1, &_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);

  // Put the verticies into the VBO
  glBufferData(GL_ARRAY_BUFFER,
               GetVertexListSizeInBytes(),
               &_verts<Vertex>[0].x,
               GL_STATIC_DRAW);

  Vertex::GetAttributes(&_attributes);

  std::vector<VertexAttributeInfo>::iterator attr_it =  _attributes.begin();

   while(attr_it != _attributes.end())
   {

      // connect the xyz to the "vert" attribute of the vertex shader
      attr_it->location  = program->getAttributeLocation(attr_it->name);
      glEnableVertexAttribArray(attr_it->location);
      glVertexAttribPointer(attr_it->location,
                            attr_it->count_of_memory_type,
                            attr_it->memory_type,
                            GL_FALSE,
                            sizeof(Vertex),
                            attr_it->buffer_offset);
      std::cout << "Setup " << attr_it->name << " Slot = " << attr_it->location << std::endl; std::cout.flush();
      glDisableVertexAttribArray(attr_it->location);
      ++it;

   }

   GLUtil::checkGLErrors();

   _uniforms.push_back(UniformInfo("modelview"));
   std::vector<UniformInfo>::iterator uni_it =  _uniforms.begin();

   while(uni_it != _uniforms.end())
   {
      uni_it->location = program.getUniformLocation(uni_it->name);
      std::cout << "Setup " << uni_it->name << " Uniform Slot = " <<
                   uni_it->location << std::endl; std::cout.flush();

      ++uni_it;
   }

   GLUtil::checkGLErrors();

   std::cout << "Done setting up triangle" << std::endl; std::cout.flush();

   GLUtil::checkGLErrors();
}

}

#endif // GLMESH_H
