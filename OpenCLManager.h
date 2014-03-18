#ifndef OPENCL_MANAGER_H
#define OPENCL_MANAGER_H

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h> 
  #include <OpenGL/CGLCurrent.h>
  #include <OpenGL/CGLTypes.h>
  #include <OpenCL/opencl.h>
  #include <OpenCL/cl_gl_ext.h>
  #include <OpenGL/CGLDevice.h>
#else
#include <CL/cl.h>
#endif

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)


class OpenCLManager 
{
  public:

  private:
    DISALLOW_COPY_AND_ASSIGN(OpenCLManager);

  public:
    OpenCLManager():_context(NULL), _device(NULL), _commandQueue(NULL){};
    ~OpenCLManager();



  protected:
    cl_device _device;
    cl_context _context;
    cl_command_queue _commandQueue;

};
#endif //OPENCL_MANAGER_H
