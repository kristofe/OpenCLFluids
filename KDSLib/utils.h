#ifndef UTILS_H
#define UTILS_H



namespace kdslib{

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

#define BUFFER_OFFSET(i) ((void*)(i))

}

#endif // UTILS_H
