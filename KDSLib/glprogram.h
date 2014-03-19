#ifndef GLPROGRAM_H
#define GLPROGRAM_H

#include "glutil.h"
#include <map>

namespace kdslib{

class GLProgram
{
//Methods
public:
  GLProgram(){};

  GLProgram(std::string& vsFileName, std::string& fsFileName)
  {
      _programID =  kdslib::GLUtil::loadShaders(vsFileName, fsFileName, "");
  }

  GLProgram(const std::string& vsFileName,
            const std::string& fsFileName,
            const std::string& gsFileName)
  {
      _programID =  kdslib::GLUtil::loadShaders(vsFileName, fsFileName, gsFileName);
  }

  void loadShaders(const std::string& vsFileName, const std::string& fsFileName)
  {
      _programID =  kdslib::GLUtil::loadShaders(vsFileName, fsFileName, "");
  }

  void loadShaders(const std::string& vsFileName,
                   const std::string& fsFileName,
                   const std::string& gsFileName)
  {
      _programID =  kdslib::GLUtil::loadShaders(vsFileName, fsFileName, gsFileName);
  }

  GLint getAttributeLocation(const std::string& name)
  {
     std::map<std::string, GLint>::iterator it = _attributeLocations.find(name);
     if(it != _attributeLocations.end()){
           return it->second;
     }

    GLint loc = glGetAttribLocation(_programID, name.c_str());
    _attributeLocations[name] = loc;
    return loc;
  }

  GLint getUniformLocation(const std::string& name)
  {
     std::map<std::string, GLint>::iterator it = _uniformLocations.find(name);
     if(it != _uniformLocations.end()){
           return it->second;
     }

    GLint loc = glGetUniformLocation(_programID, name.c_str());
    _uniformLocations[name] = loc;
    return loc;
  }

  void enableVertexAttributes()
  {
     std::map<std::string, GLint>::iterator it = _attributeLocations.begin();
     while(it != _attributeLocations.end())
     {
       glEnableVertexAttribArray(it->second);
        ++it;
     }
  }

  inline GLint getID()
  {
     return _programID;
  }


private:
  DISALLOW_COPY_AND_ASSIGN(GLProgram);

//Data Members

protected:
   GLint _programID;
   std::map<std::string, GLint> _attributeLocations;
   std::map<std::string, GLint> _uniformLocations;


};

}
#endif // GLPROGRAM_H
