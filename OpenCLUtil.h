
#ifndef OPENCL_UTIL_H
#define OPENCL_UTIL_H

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

/* An error check macro for OpenCL.
 *
 * Usage:
 * CHECK_CL_ERROR(status_code_from_a_cl_operation, "function_name")
 *
 * It will abort with a message if an error occurred.
 */

#define CHECK_CL_ERROR(STATUS_CODE, WHAT) \
  if ((STATUS_CODE) != CL_SUCCESS) \
  { \
    fprintf(stderr, \
        "*** '%s' in '%s' on line %d failed with error '%s'.\n", \
        WHAT, __FILE__, __LINE__, \
        OpenCLUtil::errorToString(STATUS_CODE)); \
    abort(); \
  }

/* A more automated error check macro for OpenCL, for use with clXxxx
 * functions that return status codes. (Not all of them do, notably 
 * clCreateXxx do not.)
 *
 * Usage:
 * CALL_CL_GUARDED(clFunction, (arg1, arg2));
 *
 * Note the slightly strange comma between the function name and the
 * argument list.
 */

#define CALL_CL_GUARDED(NAME, ARGLIST) \
  { \
    cl_int status_code; \
      status_code = NAME ARGLIST; \
    CHECK_CL_ERROR(status_code, #NAME); \
  }

/* An error check macro for Unix system functions. If "COND" is true, then the
 * last system error ("errno") is printed along with MSG, which is supposed to
 * be a string describing what you were doing.
 *
 * Example:
 * CHECK_SYS_ERROR(dave != 0, "opening hatch");
 */
#define CHECK_SYS_ERROR(COND, MSG) \
  if (COND) \
  { \
    perror(MSG); \
    abort(); \
  }

#define CHOOSE_INTERACTIVELY "INTERACTIVE"
class OpenCLUtil 
{

// static methods
  public:
    static const char *errorToString(cl_int e);
    static void printPlatformDevices();

    static void createContextOn(const char *plat_name, const char*dev_name, cl_uint idx, cl_context *ctx, cl_command_queue *queue, int enable_profiling);

    static char *readALine();
    static char *readFile(const char *filename);

    static cl_kernel kernelFromString(cl_context ctx, char const *knl, char const *knl_name, char const *options);

    static void printDeviceInfo(cl_device_id device);
    static void getDeviceNameFromQueue(cl_command_queue queue, char * buf, int bufsize);
    static void printDeviceInfoFromQueue(cl_command_queue queue);

    void setKernelArg(cl_kernel krnl, int index, size_t size, const void * arg);

//instance methods

  public:
    OpenCLUtil():_currentKernel(NULL), _currentArgIndex(0){};

    void setCurrentKernel(cl_kernel krnl) 
    { 
      _currentKernel = krnl; 
    };

    void setCurrentArgIndex(int idx)
    {
      _currentArgIndex = idx;
    };

    void setKernelArg(size_t size, const void * arg);
    void setKernelArg(int idx, size_t size, const void * arg);
      


  protected:

    cl_kernel _currentKernel;
    int       _currentArgIndex;


};

#endif //OPENCL_UTIL_H
