cmake_minimum_required  (VERSION 2.6)
project(OpenCLFluids)

set(SOURCES
   main.cpp
#cl-helper.c
   OpenCLUtil.cpp
   OpenCLManager.cpp
)
file(GLOB KDSLIB_SRC
    "KDSLIB/*.h"
    "KDSLIB/*.cpp"
)

set(HEADERS
  cl_solver.h
#  cl-helper.h
  OpenCLUtil.h
  OpenCLManager.h
  second_order_solver.h
  timing.h
)

#set(KDSLIBFILE_PATH "libKDSLib.a" CACHE FILEPATH "Where is KDSLib?")

# -g adds debugging symbols
# -Wall turns on all warnings
# -Wextra turns on a lot of warnings (but not too pedantic)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wextra -Wno-ignored-qualifiers -Wno-unused-parameter -std=c++11")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}  -g -Wextra -Wno-ignored-qualifiers -Wno-unused-parameter")

#SET(CMAKE_OSX_ARCHITECTURES "i386")


##SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g -Wextra -Wno-ignored-qualifiers -Wno-unused-parameter -std=c++11 -stdlib=libc++ -O3 -msse -msse2")
##SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g -Wextra -Wno-ignored-qualifiers -Wno-unused-parameter -O3 -msse -msse2")

if(CMAKE_BUILD_TYPE MATCHES DEBUG)
    message("adding debug flags")
    add_definitions(-DDEBUG)
    add_definitions(-D_DEBUG)
    add_definitions(-DBREAK_ON_EXCEPTION)
endif(CMAKE_BUILD_TYPE MATCHES DEBUG)


#set(KDSLIB_PATH "/Users/kristofe/Documents/Projects/KDSLib/")
set(GLFWLIB_PATH "/Users/kristofe/Documents/Projects/glfw-3.0.1/CMake/src")
#set(KDSLIB_BUILD_PATH ${KDSLIB_PATH}build)
#set(KDSLIB_INCLUDE_PATH ${KDSLIB_PATH}include)
#set(GLM_INCLUDE_PATH ${KDSLIB_PATH})

##message(${KDSLIBBUILD_PATH})

##add_subdirectory(${KDSLIB_PATH} ${KDSLIB_BUILD_PATH})
#include_directories(${KDSLIB_INCLUDE_PATH} ${GLM_INCLUDE_PATH})
link_directories(${GLFWLIB_PATH})

if(APPLE)
   FIND_LIBRARY(IOKIT_LIBRARY IOKit)
   FIND_LIBRARY(COCOA_LIBRARY Cocoa)
   FIND_LIBRARY(GLUT_LIBRARY GLUT)
   FIND_LIBRARY(OPENGL_LIBRARY OpenGL)
   FIND_LIBRARY(OPENCL_LIBRARY OpenCL)

   MARK_AS_ADVANCED(
#                     IOKIT_LIBRARY
#                     COCOA_LIBRARY
                     GLUT_LIBRARY
                     OPENGL_LIBRARY
                     OPENCL_LIBRARY
                     )
#SET(EXTRA_LIBS ${IOKIT_LIBRARY} ${COCOA_LIBRARY} ${OPENGL_LIBRARY} libKDSLib.a libglfw3.a )
   SET(EXTRA_LIBS ${IOKIT_LIBRARY} ${COCOA_LIBRARY} ${OPENCL_LIBRARY} ${OPENGL_LIBRARY} libglfw3.a)
ENDIF(APPLE)



add_executable(fluidsim ${SOURCES} ${HEADERS} ${KDSLIB_SRC})
target_link_libraries(fluidsim ${EXTRA_LIBS})
