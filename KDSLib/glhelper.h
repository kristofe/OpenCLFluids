#ifndef GLLIB_GLHELPER_H
#define GLLIB_GLHELPER_H


//#define GLFW_INCLUDE_CLCOREARB
//#include "../GLFW/glfw3.h" // - lib is in /usr/local/lib/libglfw3.a
#include <string>
#include <map>
#include <utility>
#include "glutil.h"
//#include "GLM/constants.hpp"

#ifndef DISALLOW_COPY_AND_ASSIGN
// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)
#endif

#define BUFFER_OFFSET(i) ((void*)(i))
//#define BUFFER_OFFSET(i) ((char*)NULL+(i))
namespace kdslib{

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
}

#endif //GLLIB_GLHELPER_H
