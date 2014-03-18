#include "OpenCLManager.h"
#include "OpenCLUtil.h"


OpenCLManager::OpenCLManager():
  _context(NULL), _device(NULL), _commandQueue(NULL)
{

}

OpenCLManager::~OpenCLManager()
{

}


void OpenCLManager::createContext(const char *plat_name, const char*dev_name, cl_uint idx, int enable_profiling)
{
  OpenCLUtil::createContextOn(plat_name, dev_name, idx, &_context, &_commandQueue, enable_profiling);

  //TODO: this should go into create context
  CALL_CL_GUARDED(clGetCommandQueueInfo, (_commandQueue, CL_QUEUE_DEVICE, sizeof(_device), &_device, NULL));

}

void OpenCLManager::createContextForOpenGLOSX()
{
  cl_int status;
  
  CGLContextObj gl_context = CGLGetCurrentContext();
//  const char * err = CGLErrorString(kCGLContext);
  
  CGLShareGroupObj kCGLShareGroup = CGLGetShareGroup(gl_context);
  

  cl_context_properties properties[] = {
    CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
    (cl_context_properties)kCGLShareGroup, 0
  };

  _context = clCreateContext(properties, 0, 0, 0, 0, &status);
  CHECK_CL_ERROR(status, "clCreateContext");
  
  // And now we can ask OpenCL which particular device is being used by
  // OpenGL to do the rendering, currently:
  cl_device_id renderer;
  clGetGLContextInfoAPPLE(_context, gl_context,
                          CL_CGL_DEVICE_FOR_CURRENT_VIRTUAL_SCREEN_APPLE, sizeof(renderer),
                          &renderer, NULL);
  
  cl_uint id_in_use;
  clGetDeviceInfo(renderer, CL_DEVICE_VENDOR_ID, sizeof(cl_uint),
                  &id_in_use, NULL);
  
  _device = renderer;
  
  cl_command_queue_properties qprops = 0;
  
  _commandQueue = clCreateCommandQueue(_context, _device, qprops, &status);
  CHECK_CL_ERROR(status, "clCreateCommandQueue");
} 
  

  
  
