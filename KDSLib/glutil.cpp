#include <vector>
#include <stdio.h>
#include "include/glutil.h"
#include "glm/glm.hpp"

namespace kdslib{


GLUtil::GLUtil()
{
}

GLuint GLUtil::loadShaders(const std::string& vsFileName,
                   const std::string& fsFileName,
                   const std::string& gsFileName)
{

    printf("loading shaders %s %s %s\n", vsFileName.c_str(), fsFileName.c_str(),
                                         gsFileName.c_str());
    std::string vsSource = getShaderSource(vsFileName);
    std::string fsSource = getShaderSource(fsFileName);
    if(gsFileName.length() == 0)
    {
       std::string gsSource = getShaderSource(gsFileName);
       return buildProgram(vsSource,fsSource, gsSource);
    }
    else
    {
       return buildProgram(vsSource,fsSource,NULL);
    }


}

GLuint GLUtil::buildShader(const std::string& name,
                   const std::string& source,
                   GLenum shaderType)
{
    const char* src = source.c_str();
    GLuint shaderHandle = glCreateShader(shaderType);
    glShaderSource(shaderHandle,1, &src, 0);
    glCompileShader(shaderHandle);

    GLint compileSuccess;
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compileSuccess);

    if(compileSuccess == GL_FALSE)
    {
        printf("Error(s) in shader %s:\n", name.c_str());
        GLchar messages[256];
        glGetShaderInfoLog(shaderHandle,sizeof(messages), 0, &messages[0]);
        printf("%s\n",messages);
        //exit(1);
    }
    else
    {
        printf("Successfully created shader %s!\n", name.c_str());
    }
    return shaderHandle;
}


GLuint GLUtil::buildProgram(const std::string& vsSource,
                            const std::string& fsSource,
                            const std::string& gsSource)
{
    GLuint vertexShader = buildShader("vertex shader",vsSource,
                                      GL_VERTEX_SHADER);
    GLuint fragmentShader = buildShader("fragment shader",fsSource,
                                       GL_FRAGMENT_SHADER);

    GLuint programHandle = glCreateProgram();
    glAttachShader(programHandle, vertexShader);
    glAttachShader(programHandle, fragmentShader);
    glLinkProgram(programHandle);

    GLint linkSuccess;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &linkSuccess);
    if(linkSuccess == GL_FALSE)
    {
        printf("Error(s) in program:\n");
        GLchar messages[256];
        glGetProgramInfoLog(programHandle, sizeof(messages), 0, &messages[0]);
        printf("%s\n", messages);
        //exit(1);
    }
    else
    {
        printf("Successfully created vertexprogram!\n");
    }

    printActiveAttributes(programHandle);
    printActiveUniforms(programHandle);
    return programHandle;
}

void GLUtil::printActiveUniforms(GLuint programHandle)
{
  GLint uniformCount;
  glGetProgramiv(programHandle, GL_ACTIVE_UNIFORMS, &uniformCount);

  printf("Active Uniform Count: %d\n", uniformCount);
  if(uniformCount > 0)
  {
      GLint maxUniformNameLength = 0;
      glGetProgramiv(programHandle, GL_ACTIVE_UNIFORM_MAX_LENGTH,
                     &maxUniformNameLength);
      std::vector<GLchar> nameData(maxUniformNameLength);;
      for(int uniformID = 0; uniformID < uniformCount; ++uniformID)
      {
          GLint arraySize = 0;
          GLenum  type = 0;
          GLsizei actualLength = 0;
          glGetActiveUniform(
                          programHandle,
                          uniformID,
                          nameData.size(),
                          &actualLength,
                          &arraySize,
                          &type,
                          &nameData[0]
                          );
         std::string name((char*)&nameData[0], actualLength);
         printf("Uniform %d name: %s, type:%s\n", uniformID, name.c_str(),
                        GLUtil::glEnumToString(type).c_str());
        }
  }
}

void GLUtil::getActiveUniforms(
                               GLuint programHandle,
                               std::map<std::string, ShaderUniformData>* dict
                               )
{
  GLint uniformCount;
  glGetProgramiv(programHandle, GL_ACTIVE_UNIFORMS, &uniformCount);

  printf("Active Uniform Count: %d\n", uniformCount);
  if(uniformCount > 0)
  {
      GLint maxUniformNameLength = 0;
      glGetProgramiv(programHandle, GL_ACTIVE_UNIFORM_MAX_LENGTH,
                     &maxUniformNameLength);
      std::vector<GLchar> nameData(maxUniformNameLength);;
      for(int uniformID = 0; uniformID < uniformCount; ++uniformID)
      {
          GLint arraySize = 0;
          GLenum  type = 0;
          GLsizei actualLength = 0;
          glGetActiveUniform(
                          programHandle,
                          uniformID,
                          nameData.size(),
                          &actualLength,
                          &arraySize,
                          &type,
                          &nameData[0]
                          );
          std::string name((char*)&nameData[0], actualLength);
          ShaderUniformData sud(name, type);
          std::pair<
                    std::map< std::string, ShaderUniformData>::iterator,
                    bool
                   > res;
          res = dict->insert(std::make_pair(name, sud));
          if(res.second == false)
          {
           fprintf(
                 stderr,
                 "Can't insert key '%s' into uniform dict, it already exists.\n",
                 name.c_str()
                 );
          }
        }
  }
}

void GLUtil::printActiveAttributes(GLuint programHandle)
{
  GLint attributeCount;
  glGetProgramiv(programHandle, GL_ACTIVE_ATTRIBUTES, &attributeCount);
  printf("Active Attribute Count: %d\n", attributeCount);
  if(attributeCount > 0)
  {
      GLint maxAttributeNameLength = 0;
      glGetProgramiv(programHandle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH,
                     &maxAttributeNameLength);
      std::vector<GLchar> nameData(maxAttributeNameLength);;
      for(int attrib = 0; attrib < attributeCount; ++attrib)
      {
          GLint arraySize = 0;
          GLenum type = 0;
          GLsizei actualLength = 0;
          glGetActiveAttrib(
                          programHandle,
                          attrib,
                          nameData.size(),
                          &actualLength,
                          &arraySize,
                          &type,
                          &nameData[0]
                          );
         std::string name((char*)&nameData[0], actualLength);
         printf("Attribute %d name: %s, type:%s\n", attrib, name.c_str(),
                        GLUtil::glEnumToString(type).c_str());
        }
  }
}

void GLUtil::getActiveAttributes(
                                  GLuint programHandle,
                                  std::map<std::string, ShaderAttributeData>* dict
                                  )
{
  GLint attributeCount;
  glGetProgramiv(programHandle, GL_ACTIVE_ATTRIBUTES, &attributeCount);
  printf("Active Attribute Count: %d\n", attributeCount);
  if(attributeCount > 0)
  {
      GLint maxAttributeNameLength = 0;
      glGetProgramiv(programHandle, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH,
                     &maxAttributeNameLength);
      std::vector<GLchar> nameData(maxAttributeNameLength);;
      for(int attrib = 0; attrib < attributeCount; ++attrib)
      {
          GLint arraySize = 0;
          GLenum type = 0;
          GLsizei actualLength = 0;
          glGetActiveAttrib(
                          programHandle,
                          attrib,
                          nameData.size(),
                          &actualLength,
                          &arraySize,
                          &type,
                          &nameData[0]
                          );
         std::string name((char*)&nameData[0], actualLength);
         ShaderAttributeData sd(name, type);
         std::pair<
                   std::map< std::string, ShaderAttributeData>::iterator,
                   bool
                  > res;
         res = dict->insert(std::make_pair(name, sd));
         if(res.second == false)
         {
          fprintf(
                stderr,
                "Can't insert key '%s' into attribute dict, it already exists.\n",
                name.c_str()
                );
         }
        }
  }
}

//WebGL version
/*
    GLint linkSuccess;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &linkSuccess);
    if(linkSuccess == GL_FALSE)
    {
        printf("Error(s) in program:\n");
        GLchar messages[256];
        glGetProgramInfoLog(programHandle, sizeof(messages), 0, &messages[0]);
        printf("%s\n", messages);
        //exit(1);
    }
    else
    {
        printf("Successfully created vertexprogram!\n");
    }
*/

std::string GLUtil::getShaderSource(const std::string& filename)
{
  std::cout << "trying to read " << filename << std::endl; std::cout.flush();
  std::ifstream in(filename.c_str(), std::ios::in | std::ios::binary);
  if (in)
  {
    std::string contents;
    in.seekg(0, std::ios::end);
    contents.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&contents[0], contents.size());
    in.close();
    std::cout << filename << std::endl << contents << std::endl; std::cout.flush();
    return(contents);
  }
  return "";
}

std::string GLUtil::getOpenGLInfo()
{
   std::stringstream s;
   s << "GL_VENDOR: " << (const char*)glGetString(GL_VENDOR) << std::endl;
   s << "GL_RENDERER: " << glGetString(GL_RENDERER) << std::endl;
   s << "GL_VERSION: " << glGetString(GL_VERSION) << std::endl;
   s << "GL_SHADING_LANGUAGE_VERSION: " <<
        glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

   return s.str();
}

int GLUtil::checkGLErrors()
{
  int errCount = 0;
  for(GLenum currError = glGetError();
      currError != GL_NO_ERROR;
      currError = glGetError())
  {
    //Do something with `currError`.
    std::cout << "GL ERROR: " << GLUtil::glEnumToString(currError) << std::endl;
    std::cout.flush();

    ++errCount;
  }

  return errCount;
}

std::string GLUtil::glEnumToString(GLenum e)
{
  std::string str = "UNKNOWN";
  switch(e)
    {

      //Data Types
      case GL_FLOAT:
        str =  "GL_FLOAT";
        break;
      case GL_FLOAT_VEC2:
        str =  "GL_FLOAT_VEC2";
        break;
      case GL_FLOAT_VEC3:
        str =  "GL_FLOAT_VEC3";
        break;
      case GL_FLOAT_VEC4:
        str =  "GL_FLOAT_VEC4";
        break;
      case GL_FLOAT_MAT2:
        str =  "GL_FLOAT_MAT2";
        break;
      case GL_FLOAT_MAT3:
        str =  "GL_FLOAT_MAT3";
        break;
      case GL_FLOAT_MAT4:
        str =  "GL_FLOAT_MAT4";
        break;
      case GL_FLOAT_MAT2x3:
        str =  "GL_FLOAT_MAT2x3";
        break;
      case GL_FLOAT_MAT2x4:
        str =  "GL_FLOAT_MAT2x4";
        break;
      case GL_FLOAT_MAT3x2:
        str =  "GL_FLOAT_MAT3x2";
        break;
      case GL_FLOAT_MAT3x4:
        str =  "GL_FLOAT_MAT3x4";
        break;
      case GL_FLOAT_MAT4x2:
        str =  "GL_FLOAT_MAT4x2";
        break;
      case GL_FLOAT_MAT4x3:
        str =  "GL_FLOAT_MAT4x3";
        break;
      case GL_INT:
        str =  "GL_INT";
        break;
      case GL_INT_VEC2:
        str =  "GL_INT_VEC2";
        break;
      case GL_INT_VEC3:
        str =  "GL_INT_VEC3";
        break;
      case GL_INT_VEC4:
        str =  "GL_INT_VEC4";
        break;
      case GL_UNSIGNED_INT:
        str =  "GL_UNSIGNED_INT";
        break;
      case GL_UNSIGNED_INT_VEC2:
        str =  "GL_UNSIGNED_INT_VEC2";
        break;
      case GL_UNSIGNED_INT_VEC3:
        str =  "GL_UNSIGNED_INT_VEC3";
        break;
      case GL_UNSIGNED_INT_VEC4:
        str =  "GL_UNSIGNED_INT_VEC4";
        break;

      //Error Codes
      case GL_NO_ERROR:
        str =  "GL_NO_ERROR";
        break;
      case GL_INVALID_ENUM:
        str =  "GL_INVALID_ENUM";
        break;
      case GL_INVALID_VALUE:
        str =  "GL_INVALID_VALUE";
        break;
      case GL_INVALID_OPERATION:
        str =  "GL_INVALID_OPERATION";
        break;
      case GL_INVALID_FRAMEBUFFER_OPERATION:
        str =  "GL_INVALID_FRAMEBUFFER_OPERATION";
        break;
      case GL_OUT_OF_MEMORY:
        str =  "GL_OUT_OF_MEMORY";
        break;


        /* GL 4 enums?
      case GL_STACK_UNDERFLOW:
        str =  "GL_STACK_UNDERFLOW";
        break;
      case GL_STACK_OVERFLOW:
        str =  "GL_STACK_OVERFLOW";
        break;

      case DOUBLE:
        str =  "DOUBLE";
        break;
      case DOUBLE_VEC2:
        str =  "DOUBLE_VEC2";
        break;
      case DOUBLE_VEC3:
        str =  "DOUBLE_VEC3";
        break;
      case DOUBLE_VEC4:
        str =  "DOUBLE_VEC4";
        break;
      case DOUBLE_MAT2:
        str =  "DOUBLE_MAT2";
        break;
      case DOUBLE_MAT3:
        str =  "DOUBLE_MAT3";
        break;
      case DOUBLE_MAT4:
        str =  "DOUBLE_MAT4";
        break;
      case DOUBLE_MAT2x3:
        str =  "DOUBLE_MAT2x3";
        break;
      case DOUBLE_MAT2x4:
        str =  "DOUBLE_MAT2x4";
        break;
      case DOUBLE_MAT3x2:
        str =  "DOUBLE_MAT3x2";
        break;
      case DOUBLE_MAT3x4:
        str =  "DOUBLE_MAT3x4";
        break;
      case DOUBLE_MAT4x2:
        str =  "DOUBLE_MAT4x2";
        break;
      case DOUBLE_MAT4x3
        str =  "DOUBLE_MAT4x3";
        break;
      */
    }

    return str;
}

}


/*s
typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef signed char GLbyte;
typedef short GLshort;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned int GLuint;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void GLvoid;
*/

/*

void drawQuadVBO() {
    glUniform1f(blendSlot, blendStrength);
    glEnableVertexAttribArray(positionSlot);
    glEnableVertexAttribArray(uvSlot);
    glUniform1i(samplerSlot,0);

    glUniform1iv(redMapSlot,sizeof(mapR),mapR);
    glUniform1iv(greenMapSlot,sizeof(mapG),mapG);
    glUniform1iv(blueMapSlot,sizeof(mapB),mapB);

    glUniform4fv(flareColor1Slot,1,flareColor1);
    glUniform2fv(dxdy1Slot,1,dxdy1);
    glUniform3fv(scale1Slot,1,scale1);

    glUniform4fv(flareColor2Slot,1,flareColor2);
    glUniform2fv(dxdy2Slot,1,dxdy2);
    glUniform3fv(scale2Slot,1,scale2);

    printf("imageDims(%3.2f,%3.2f)\n", imageDims[0],imageDims[1]);
    glUniform2fv(imageDimensionsSlot,1,imageDims);

    glBindBuffer(GL_ARRAY_BUFFER, vboQuad);

    glUniformMatrix4fv(projectionUniformSlot, 1, 0, projectionMatrix);
    glUniformMatrix4fv(modelViewUniformSlot, 1, 0, modelviewMatrix);
    glVertexAttribPointer(positionSlot, 2, GL_FLOAT, GL_FALSE, sizeof(V2fT2f),
                          BUFFER_OFFSET(0));
    glVertexAttribPointer(uvSlot, 2, GL_FLOAT, GL_FALSE, sizeof(V2fT2f),
                          BUFFER_OFFSET(sizeof(float)*2));



    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboQuadIndices);
    glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_BYTE, 0);

    glDisableVertexAttribArray(positionSlot);
    glDisableVertexAttribArray(uvSlot);

}
*/

/*
    // New Stuff

    // Load shader and setup its parameters
    firstPassShaderProgram = loadShaders();
    glUseProgram(firstPassShaderProgram);

    positionSlot = glGetAttribLocation(firstPassShaderProgram, "Position");
    uvSlot = glGetAttribLocation(firstPassShaderProgram, "uv0");

    glEnableVertexAttribArray(positionSlot);
    glEnableVertexAttribArray(uvSlot);

    projectionUniformSlot = glGetUniformLocation(firstPassShaderProgram, "Projection");
    modelViewUniformSlot = glGetUniformLocation(firstPassShaderProgram, "Modelview");
    samplerSlot = glGetUniformLocation(firstPassShaderProgram, "s_texture");
    imageDimensionsSlot = glGetUniformLocation(firstPassShaderProgram, "u_imageDimensions");
    redMapSlot = glGetUniformLocation(firstPassShaderProgram, "u_redmap");
    greenMapSlot = glGetUniformLocation(firstPassShaderProgram, "u_greenmap");
    blueMapSlot = glGetUniformLocation(firstPassShaderProgram, "u_bluemap");
    flareColor1Slot = glGetUniformLocation(firstPassShaderProgram, "u_flareColor1");
    dxdy1Slot = glGetUniformLocation(firstPassShaderProgram, "u_dxdy1");
    scale1Slot = glGetUniformLocation(firstPassShaderProgram, "u_scale1");

    flareColor2Slot = glGetUniformLocation(firstPassShaderProgram, "u_flareColor2");
    dxdy2Slot = glGetUniformLocation(firstPassShaderProgram, "u_dxdy2");
    scale2Slot = glGetUniformLocation(firstPassShaderProgram, "u_scale2");

    blendSlot = glGetUniformLocation(firstPassShaderProgram, "u_blend");

    // Generate VBO
    glGenBuffers(1,&vboQuad);
    glBindBuffer(GL_ARRAY_BUFFER, vboQuad);
    glBufferData(GL_ARRAY_BUFFER, sizeof(flipquad),&flipquad[0].x,GL_STATIC_DRAW);

    glGenBuffers(1, &vboQuadIndices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboQuadIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices[0], GL_STATIC_DRAW);

    Ortho(projectionMatrix,0, 1, 0, 1, -1, 1);
    Scaling(modelviewMatrix,1, 1, 1);
 */


/*
void bindFBO(GLuint fboID,Image* fboImage) {
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, fboID);
    glViewport(0, 0, fboImage->wide*fboImage->s, fboImage->high*fboImage->t);
    // The entire framebuffer won't be written to if the image was padded to POT.
    // In this case, clearing is a performance win on TBDR systems.
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_BLEND);
    glViewport(0, 0, fboImage->wide, fboImage->high);
}

void restoreSystemFBO() {
    glBindFramebufferOES(GL_FRAMEBUFFER_OES, SystemFBO);
}

void drawQuadArray() {
    // Unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUniform1f(blendSlot, blendStrength);
    glUniformMatrix4fv(projectionUniformSlot, 1, 0, projectionMatrix);
    glUniformMatrix4fv(modelViewUniformSlot, 1, 0, modelviewMatrix);
    glVertexAttribPointer(positionSlot, 2, GL_FLOAT, GL_FALSE, sizeof(V2fT2f),  &flipquad[0].x);
    glVertexAttribPointer(uvSlot, 2, GL_FLOAT, GL_FALSE, sizeof(V2fT2f),  &flipquad[0].s);
    glUniform1i(samplerSlot,0);
    glUniform1iv(redMapSlot,sizeof(mapR),mapR);
    glUniform1iv(greenMapSlot,sizeof(mapG),mapG);
    glUniform1iv(blueMapSlot,sizeof(mapB),mapB);

    glUniform4fv(flareColor1Slot,1,flareColor1);
    glUniform2fv(dxdy1Slot,1,dxdy1);
    glUniform3fv(scale1Slot,1,scale1);

    glUniform4fv(flareColor2Slot,1,flareColor2);
    glUniform2fv(dxdy2Slot,1,dxdy2);
    glUniform3fv(scale2Slot,1,scale2);

    glUniform2fv(imageDimensionsSlot,1,imageDims);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}
*/
/*
//////////////////////////////
inline void drawCube(Vector3f &p1, Vector3f &p2) {

glBegin(GL_QUADS);
		// Front Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(p1.x, p1.y,  p2.z);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f( p2.x, p1.y,  p2.z);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f( p2.x,  p2.y,  p2.z);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(p1.x,  p2.y,  p2.z);	// Top Left Of The Texture and Quad
		// Back Face
		glTexCoord2f(1.0f, 0.0f); glVertex3f(p1.x, p1.y, p1.z);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(p1.x,  p2.y, p1.z);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f( p2.x,  p2.y, p1.z);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f( p2.x, p1.y, p1.z);	// Bottom Left Of The Texture and Quad
		// Top Face
		glTexCoord2f(0.0f, 1.0f); glVertex3f(p1.x,  p2.y, p1.z);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f(p1.x,  p2.y,  p2.z);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f( p2.x,  p2.y,  p2.z);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f( p2.x,  p2.y, p1.z);	// Top Right Of The Texture and Quad
		// Bottom Face
		glTexCoord2f(1.0f, 1.0f); glVertex3f(p1.x, p1.y, p1.z);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f( p2.x, p1.y, p1.z);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f( p2.x, p1.y,  p2.z);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(p1.x, p1.y,  p2.z);	// Bottom Right Of The Texture and Quad
		// Right face
		glTexCoord2f(1.0f, 0.0f); glVertex3f( p2.x, p1.y, p1.z);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f( p2.x,  p2.y, p1.z);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f( p2.x,  p2.y,  p2.z);	// Top Left Of The Texture and Quad
		glTexCoord2f(0.0f, 0.0f); glVertex3f( p2.x, p1.y,  p2.z);	// Bottom Left Of The Texture and Quad
		// Left Face
		glTexCoord2f(0.0f, 0.0f); glVertex3f(p1.x, p1.y, p1.z);	// Bottom Left Of The Texture and Quad
		glTexCoord2f(1.0f, 0.0f); glVertex3f(p1.x, p1.y,  p2.z);	// Bottom Right Of The Texture and Quad
		glTexCoord2f(1.0f, 1.0f); glVertex3f(p1.x,  p2.y,  p2.z);	// Top Right Of The Texture and Quad
		glTexCoord2f(0.0f, 1.0f); glVertex3f(p1.x,  p2.y, p1.z);	// Top Left Of The Texture and Quad
	glEnd();

}
*/
