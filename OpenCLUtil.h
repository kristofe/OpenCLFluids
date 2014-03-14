
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
        OpenCLUtil::cl_error_to_str(STATUS_CODE)); \
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

#define SET_1_KERNEL_ARG(knl, arg0) \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 0, sizeof(arg0), &arg0));

#define SET_2_KERNEL_ARGS(knl, arg0, arg1) \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 0, sizeof(arg0), &arg0)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 1, sizeof(arg1), &arg1));

#define SET_3_KERNEL_ARGS(knl, arg0, arg1, arg2) \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 0, sizeof(arg0), &arg0)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 1, sizeof(arg1), &arg1)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 2, sizeof(arg2), &arg2));

#define SET_4_KERNEL_ARGS(knl, arg0, arg1, arg2, arg3) \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 0, sizeof(arg0), &arg0)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 1, sizeof(arg1), &arg1)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 2, sizeof(arg2), &arg2)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 3, sizeof(arg3), &arg3));

#define SET_5_KERNEL_ARGS(knl, arg0, arg1, arg2, arg3, arg4) \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 0, sizeof(arg0), &arg0)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 1, sizeof(arg1), &arg1)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 2, sizeof(arg2), &arg2)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 3, sizeof(arg3), &arg3)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 4, sizeof(arg4), &arg4));

#define SET_6_KERNEL_ARGS(knl, arg0, arg1, arg2, arg3, arg4, arg5) \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 0, sizeof(arg0), &arg0)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 1, sizeof(arg1), &arg1)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 2, sizeof(arg2), &arg2)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 3, sizeof(arg3), &arg3)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 4, sizeof(arg4), &arg4)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 5, sizeof(arg5), &arg5));

#define SET_7_KERNEL_ARGS(knl, arg0, arg1, arg2, arg3, arg4, arg5, arg6) \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 0, sizeof(arg0), &arg0)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 1, sizeof(arg1), &arg1)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 2, sizeof(arg2), &arg2)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 3, sizeof(arg3), &arg3)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 4, sizeof(arg4), &arg4)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 5, sizeof(arg5), &arg5)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 6, sizeof(arg6), &arg6));

#define SET_8_KERNEL_ARGS(knl, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7) \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 0, sizeof(arg0), &arg0)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 1, sizeof(arg1), &arg1)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 2, sizeof(arg2), &arg2)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 3, sizeof(arg3), &arg3)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 4, sizeof(arg4), &arg4)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 5, sizeof(arg5), &arg5)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 6, sizeof(arg6), &arg6)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 7, sizeof(arg7), &arg7));

#define SET_9_KERNEL_ARGS(knl, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8) \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 0, sizeof(arg0), &arg0)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 1, sizeof(arg1), &arg1)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 2, sizeof(arg2), &arg2)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 3, sizeof(arg3), &arg3)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 4, sizeof(arg4), &arg4)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 5, sizeof(arg5), &arg5)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 6, sizeof(arg6), &arg6)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 7, sizeof(arg7), &arg7)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 8, sizeof(arg8), &arg8));

#define SET_10_KERNEL_ARGS(knl, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9) \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 0, sizeof(arg0), &arg0)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 1, sizeof(arg1), &arg1)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 2, sizeof(arg2), &arg2)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 3, sizeof(arg3), &arg3)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 4, sizeof(arg4), &arg4)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 5, sizeof(arg5), &arg5)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 6, sizeof(arg6), &arg6)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 7, sizeof(arg7), &arg7)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 8, sizeof(arg8), &arg8)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 9, sizeof(arg9), &arg9));

#define SET_11_KERNEL_ARGS(knl, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10) \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 0, sizeof(arg0), &arg0)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 1, sizeof(arg1), &arg1)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 2, sizeof(arg2), &arg2)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 3, sizeof(arg3), &arg3)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 4, sizeof(arg4), &arg4)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 5, sizeof(arg5), &arg5)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 6, sizeof(arg6), &arg6)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 7, sizeof(arg7), &arg7)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 8, sizeof(arg8), &arg8)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 9, sizeof(arg9), &arg9)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 10, sizeof(arg10), &arg10));

#define SET_12_KERNEL_ARGS(knl, arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8, arg9, arg10, arg11) \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 0, sizeof(arg0), &arg0)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 1, sizeof(arg1), &arg1)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 2, sizeof(arg2), &arg2)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 3, sizeof(arg3), &arg3)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 4, sizeof(arg4), &arg4)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 5, sizeof(arg5), &arg5)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 6, sizeof(arg6), &arg6)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 7, sizeof(arg7), &arg7)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 8, sizeof(arg8), &arg8)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 9, sizeof(arg9), &arg9)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 10, sizeof(arg10), &arg10)); \
  CALL_CL_GUARDED(clSetKernelArg, (knl, 11, sizeof(arg11), &arg11));

#define CHOOSE_INTERACTIVELY "INTERACTIVE"
class OpenCLUtil 
{
// Methods
  public:
    static const char *cl_error_to_str(cl_int e);
    static void print_platforms_devices();

    static void create_context_on(const char *plat_name, const char*dev_name, cl_uint idx, cl_context *ctx, cl_command_queue *queue, int enable_profiling);

    static char *read_a_line();
    static char *read_file(const char *filename);

    static cl_kernel kernel_from_string(cl_context ctx, char const *knl, char const *knl_name, char const *options);

    static void print_device_info(cl_device_id device);
    static void get_device_name_from_queue(cl_command_queue queue, char * buf, int bufsize);
    static void print_device_info_from_queue(cl_command_queue queue);


};

#endif //OPENCL_UTIL_H
