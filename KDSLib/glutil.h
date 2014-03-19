#ifndef KDSLIB_GLUTIL_H
#define KDSLIB_GLUTIL_H

#ifdef WIN32
   #include <GL/gl.h>
#else
   #include <OpenGL/gl3.h>
#endif
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <map>
#include "utils.h"

//These have to be included after utils.h because of the
// MACRO DISALLOW_COPY_AND_ASSIGN
#include "vector3.h"
#include "vector2.h"
#include "color.h"

namespace kdslib
{
  struct ShaderUniformData
  {
    public:
      std::string name;
      GLenum      type;
      GLint       location;
      void*       data;

    public:
      ShaderUniformData(
                       std::string pname,
                       GLenum ptype
                       ):
                       name(pname),
                       type(ptype),
                       location(-1),
                       data(NULL)
      {
      }
      ShaderUniformData(const ShaderUniformData& other)
      {
        this->name = other.name;
        this->type = other.type;
        this->location = other.location;
        this->data = other.data;
      }

      void operator=(const ShaderUniformData& other)
      {
        this->name = other.name;
        this->type = other.type;
        this->location = other.location;
        this->data = other.data;
      }
  };

  struct ShaderAttributeData
  {
    public:
      std::string name; //Corresponds to a name of a data stream
      GLenum      type;

    public:
      ShaderAttributeData(
                       std::string pname,
                       GLenum ptype
                       ):
                       name(pname),
                       type(ptype)
      {
      }
      ShaderAttributeData(const ShaderAttributeData& other)
      {
        this->name = other.name;
        this->type = other.type;
      }

      void operator=(const ShaderAttributeData& other)
      {
        this->name = other.name;
        this->type = other.type;
      };
};

   class GLUtil
   {
   public:
     GLUtil();
     static GLuint buildShader(const std::string& name,
                               const std::string& source, GLenum shaderType);

     static GLuint buildProgram(const std::string& vsSource,
                                const std::string& fsSource,
                                const std::string& gsSource);

     static std::string getShaderSource(const std::string& filename);

     static GLuint loadShaders(const std::string& vsFileName,
                               const std::string& fsFileName,
                               const std::string& gsFileName);

     static void printActiveUniforms(GLuint programHandle);
     static void getActiveUniforms(
                            GLuint programHandle,
                            std::map<std::string, ShaderUniformData>* dict
                            );

     static void printActiveAttributes(GLuint programHandle);
     static void getActiveAttributes(
                            GLuint programHandle,
                            std::map<std::string, ShaderAttributeData>* dict
                            );


     static std::string glEnumToString(GLenum e);
     static std::string getOpenGLInfo();
     static int checkGLErrors();

   //private:
   //  std::map<std::string, unsigned int>		mStringToEnumDB;

   };




}

#endif // KDSLIB_GLUTIL_H
