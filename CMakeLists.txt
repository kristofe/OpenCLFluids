cmake_minimum_required  (VERSION 2.8)

set(app_name fluids)
project(${app_name})

#the FILE GLOB commands create lists of files according to the regex and then
#put them into the variable
#it is like set(Sources but using patterns
#FILE(GLOB LIBPNG_SOURCE source/renderlib/libpng/*.c)
#FILE(GLOB LIBPNG_HEADERS source/renderlib/libpng/*.h)
FILE(GLOB MAIN_SOURCE *.c)
FILE(GLOB MAIN_HEADERS *.h)


set(SOURCES
   ${MAIN_SOURCE}
)


set(HEADERS
  ${MAIN_HEADERS}
)


set(RESOURCES
)

set(ASSETS
#    assets/tiles.png
#    assets/pond_xneg.png
#    assets/pond_xpos.png
#    assets/pond_ypos.png
#    assets/pond_yneg.png
#    assets/pond_zneg.png
#    assets/pond_zpos.png
   )
#creates folders in the IDE's project explorer.  Better than everything
#showing up in SOURCE FILES
#source_group("soil" FILES ${SOIL_SOURCE} ${SOIL_HEADERS})

# -g adds debugging symbols
# -Wall turns on all warnings
# -Wextra turns on a lot of warnings (but not too pedantic)
if(APPLE)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wextra -Wno-ignored-qualifiers -Wno-unused-parameter -std=c++11")
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -g -Wextra -Wno-ignored-qualifiers -Wno-unused-parameter")
  add_definitions(-DTARGETMAC)
#  include_directories(source/PlatformClasses/Apple/MacClasses)
#  LIST(APPEND SOURCES source/PlatformClasses/Apple/MacClasses/Platform_Mac.mm)
#  LIST(APPEND HEADERS source/PlatformClasses/Apple/MacClasses/Platform_Mac.h)

#  link_directories(${CMAKE_CURRENT_SOURCE_DIR}/GLFW) #had to put here because the libs can conflict on linux
ENDIF(APPLE)

if(WIN32)
  include_directories(freeglut/include)
  link_directories(${CMAKE_CURRENT_SOURCE_DIR}/freeglut/lib) 
#  include_directories(glew/include)
#  LIST(APPEND SOURCES source/PlatformClasses/Windows/Platform_Win32.cpp)
#  LIST(APPEND HEADERS source/PlatformClasses/Windows/Platform_Win32.h)
#  include_directories(source/PlatformClasses/Windows)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR})
  add_definitions(-D_CRT_SECURE_NO_WARNINGS) #disable sprintf deprecation
#  link_directories(${CMAKE_CURRENT_SOURCE_DIR}/GLFW) #had to put here because the libs can conflict on linux
ENDIF(WIN32)

#SET(CMAKE_OSX_ARCHITECTURES "i386")



if(CMAKE_BUILD_TYPE MATCHES DEBUG)
    message("adding debug flags")
    add_definitions(-DDEBUG)
    add_definitions(-D_DEBUG)
    add_definitions(-DBREAK_ON_EXCEPTION)
endif(CMAKE_BUILD_TYPE MATCHES DEBUG)



#add_subdirectory(renderlib) #requires the directory to have a CMakeLists.txt file
#include_directories(source)


if(APPLE)
   FIND_LIBRARY(IOKIT_LIBRARY IOKit)
   FIND_LIBRARY(COCOA_LIBRARY Cocoa)
   FIND_LIBRARY(OPENGL_LIBRARY OpenGL)
   FIND_LIBRARY(CORE_MIDI_LIBRARY CoreMidi)

   MARK_AS_ADVANCED(
                     IOKIT_LIBRARY
                     COCOA_LIBRARY
                     OPENGL_LIBRARY
                     CORE_MIDI_LIBRARY
                   )
  LIST(
      APPEND
      EXTRA_LIBS
      ${IOKIT_LIBRARY}
      ${COCOA_LIBRARY}
      ${OPENGL_LIBRARY}
      ${CORE_MIDI_LIBRARY}
#libglfw3.a
      )
ENDIF(APPLE)

if(WIN32)
  find_package(OpenGL REQUIRED)
  include_directories(${OpenGL_INCLUDE_DIRS})
  link_directories(${OpenGL_LIBRARY_DIRS})
  add_definitions(${OpenGL_DEFINITIONS})
  if(NOT OPENGL_FOUND)
      message(ERROR " OPENGL not found!")
  endif(NOT OPENGL_FOUND)

  #find_package(GLUT REQUIRED)
  #include_directories(${GLUT_INCLUDE_DIRS})
  #link_directories(${GLUT_LIBRARY_DIRS})
  #add_definitions(${GLUT_DEFINITIONS})
  #if(NOT GLUT_FOUND)
  #    message(ERROR " GLUT not found!")
  #endif(NOT GLUT_FOUND)

  find_package(OpenCL REQUIRED)
  include_directories(${OpenCL_INCLUDE_DIRS})
  link_directories(${OpenCL_LIBRARY_DIRS})
  add_definitions(${OpenCL_DEFINITIONS})
  if(NOT OpenCL_FOUND)
      message(ERROR " OpenCL not found!")
  endif(NOT OpenCL_FOUND)
  message("OpenCL include:" ${OpenCL_INCLUDE_DIRS})
  message("OpenCL lib:" ${OpenCL_LIBRARY_DIRS})

#cmake does not like if you add .lib to the libraries on windows!
  LIST(
      APPEND
      EXTRA_LIBS
      winmm
      freeglut
      )
  set (CMAKE_C_STANDARD 99)
ENDIF(WIN32)

if(UNIX AND NOT APPLE)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wextra -Wno-ignored-qualifiers -Wno-unused-parameter -std=c++11")
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -g -Wextra -Wno-ignored-qualifiers -Wno-unused-parameter -std=c99")
  add_definitions(-DTARGEUNIX)
  add_definitions(-D_POSIX_C_SOURCE=199309) #To get timing.h to include <time.h>
#  include_directories(source/PlatformClasses/Unix)

###############################################
#THESE HAD TO BE ALL CAPS TO WORK
###############################################
  find_package(OpenGL REQUIRED)
  include_directories(${OpenGL_INCLUDE_DIRS})
  link_directories(${OpenGL_LIBRARY_DIRS})
  add_definitions(${OpenGL_DEFINITIONS})
  if(NOT OPENGL_FOUND)
      message(ERROR " OPENGL not found!")
  endif(NOT OPENGL_FOUND)

  find_package(GLUT REQUIRED)
  include_directories(${GLUT_INCLUDE_DIRS})
  link_directories(${GLUT_LIBRARY_DIRS})
  add_definitions(${GLUT_DEFINITIONS})
  if(NOT GLUT_FOUND)
      message(ERROR " GLUT not found!")
  endif(NOT GLUT_FOUND)

  #find_package(OpenCL REQUIRED)
  #HACK PATHS - Home machine has targets dir. School doesn't
  if(EXISTS "/usr/local/cuda/lib64/libOpenCL.so")
    SET(OPENCL_INCLUDE_DIRS "/usr/local/cuda/include")
    SET(OPENCL_LIBRARY_DIRS "/usr/local/cuda/lib64/")
  else()
    SET(OPENCL_INCLUDE_DIRS "/usr/local/cuda/targets/x86_64-linux/include")
    SET(OPENCL_LIBRARY_DIRS "/usr/local/cuda/targets/x86_64-linux/lib/")
  endif()
  include_directories(${OPENCL_INCLUDE_DIRS})
  link_directories(${OpenCL_LIBRARY_DIRS})
  #add_definitions(${OpenCL_DEFINITIONS})
  if(NOT OPENCL_FOUND)
      message(ERROR " OpenCL not found!")
  endif(NOT OPENCL_FOUND)

  LIST(
      APPEND
      EXTRA_LIBS
      m
      ${OPENGL_LIBRARY} 
      ${GLUT_LIBRARY}
      OpenCL
      )
ENDIF(UNIX AND NOT APPLE)


#Setting up target and it's properties
if(APPLE)
#CREATING A BUNDLE SEEMS TO DISABLE RETINA SUPPORT FOR GLFW!!!!
#  add_executable(${app_name} MACOSX_BUNDLE ${SOURCES} ${HEADERS} ${SHADERS} ${ASSETS} ${RESOURCES})
  add_executable(${app_name} ${SOURCES} ${HEADERS} ${SHADERS} ${ASSETS} ${RESOURCES})
#this is where to put the dylibs
  SET_SOURCE_FILES_PROPERTIES(

      PROPERTIES
      MACOSX_PACKAGE_LOCATION MacOSX
      )
#you have to mark each file as a resource
  foreach( rez ${RESOURCES} )
    message(${rez})
    SET_TARGET_PROPERTIES(${APP_NAME} PROPERTIES RESOURCE "${rez}")
  endforeach( rez )
  message("RESOURCES: " ${RESOURCES})

#  foreach( shader ${SHADERS} )
#    message(${shader})
#    SET_TARGET_PROPERTIES(${APP_NAME} PROPERTIES RESOURCE "${shader}")
#  endforeach( shader )
#  message("Shaders: " ${SHADERS})

  foreach( asset ${ASSETS} )
    message(${asset})
    SET_TARGET_PROPERTIES(${APP_NAME} PROPERTIES RESOURCE "${asset}")
  endforeach( asset )

#this is where to put the resources
  message("Resources: " ${RESOURCES})
  SET_SOURCE_FILES_PROPERTIES(
      ${RESOURCES}
      PROPERTIES
      MACOSX_PACKAGE_LOCATION Resources
      )
  message("Assets: " ${ASSETS})
  SET_SOURCE_FILES_PROPERTIES(
      ${ASSETS}
      PROPERTIES
      MACOSX_PACKAGE_LOCATION Resources/assets
      )
#  SET_SOURCE_FILES_PROPERTIES(
#      ${SHADERS}
#      PROPERTIES
#      MACOSX_PACKAGE_LOCATION Resources/Shaders
#      )
else()
  #add_executable(${app_name} ${SOURCES} ${HEADERS} ${SHADERS} ${RESOURCES})
  message("Sources:" ${SOURCES})
  add_executable(${app_name} ${SOURCES})
endif(APPLE)

target_link_libraries(${app_name} ${EXTRA_LIBS})
