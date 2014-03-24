#ifndef GLLIB_GLHELPER_H
#define GLLIB_GLHELPER_H

#ifdef WIN32
   #include <GL/gl.h>
#else
   #include <OpenGL/gl3.h>
#endif

//#include "../GLFW/glfw3.h" // - lib is in /usr/local/lib/libglfw3.a
#include <string>
#include <map>
#include <utility>
#include "GLM/constants.hpp"

class GLHelper {
//Typedefs and Enums
//Constants (static const data members)
//Constructors
  public:
    GLHelper()
    {
      initEnumTranslationData();

    };

//Destructor
    ~GLHelper()
    {

    };

//Methods, including static methods
    void initEnumTranslationData();
    inline std::string glEnumToString(GLenum e){ return enumToString[e];};
    inline GLenum glStringToEnum(std::string s){ return stringToEnum[s];};

//Data Members (except static const data members)
    std::map<GLenum, std::string> enumToString;
    std::map<std::string, GLenum> stringToEnum;

};
#endif //GLLIB_GLHELPER_H
