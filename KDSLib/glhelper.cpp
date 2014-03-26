#include "glhelper.h" 
namespace kdslib{

  void GLHelper::initEnumTranslationData()
  {
    using std::pair;
    using std::string;
    pair<GLenum, string> pairs [] = {
        pair<GLenum,string>( GL_ARRAY_BUFFER, "GL_ARRAY_BUFFER"),
        //pair<GLenum,string>(  GL_ATOMIC_COUNTER_BUFFER, "GL_ATOMIC_COUNTER_BUFFER"),
        pair<GLenum,string>(  GL_COPY_READ_BUFFER, "GL_COPY_READ_BUFFER"),
        pair<GLenum,string>(  GL_COPY_WRITE_BUFFER, "GL_COPY_WRITE_BUFFER"),
        pair<GLenum,string>(  GL_DRAW_INDIRECT_BUFFER, "GL_DRAW_INDIRECT_BUFFER"),
        //pair<GLenum,string>(  GL_DISPATCH_INDIRECT_BUFFER, "GL_DISPATCH_INDIRECT_BUFFER"),
        pair<GLenum,string>(  GL_ELEMENT_ARRAY_BUFFER, "GL_ELEMENT_ARRAY_BUFFER"),
        pair<GLenum,string>(  GL_PIXEL_PACK_BUFFER, "GL_PIXEL_PACK_BUFFER"),
        pair<GLenum,string>(  GL_PIXEL_UNPACK_BUFFER, "GL_PIXEL_UNPACK_BUFFER"),
        //pair<GLenum,string>(  GL_QUERY_BUFFER, "GL_QUERY_BUFFER"),
        //pair<GLenum,string>(  GL_SHADER_STORAGE_BUFFER, "GL_SHADER_STORAGE_BUFFER"),
        pair<GLenum,string>(  GL_TEXTURE_BUFFER, "GL_TEXTURE_BUFFER"),
        pair<GLenum,string>(  GL_TRANSFORM_FEEDBACK_BUFFER, "GL_TRANSFORM_FEEDBACK_BUFFER"),
        pair<GLenum,string>(  GL_UNIFORM_BUFFER, "GL_UNIFORM_BUFFER"),
        pair<GLenum,string>( GL_STREAM_DRAW, "GL_STREAM_DRAW"),
        pair<GLenum,string>(  GL_STREAM_READ, "GL_STREAM_READ"),
        pair<GLenum,string>(  GL_STREAM_COPY, "GL_STREAM_COPY"),
        pair<GLenum,string>(  GL_STATIC_DRAW, "GL_STATIC_DRAW"),
        pair<GLenum,string>(  GL_STATIC_READ, "GL_STATIC_READ"),
        pair<GLenum,string>(  GL_STATIC_COPY, "GL_STATIC_COPY"),
        pair<GLenum,string>(  GL_DYNAMIC_DRAW, "GL_DYNAMIC_DRAW"),
        pair<GLenum,string>(  GL_DYNAMIC_READ, "GL_DYNAMIC_READ"),
        pair<GLenum,string>(  GL_DYNAMIC_COPY, "GL_DYNAMIC_COPY"),
        pair<GLenum,string>( GL_FLOAT,  "GL_FLOAT"),
        pair<GLenum,string>( GL_FLOAT_VEC2,  "GL_FLOAT_VEC2"),
        pair<GLenum,string>( GL_FLOAT_VEC3,  "GL_FLOAT_VEC3"),
        pair<GLenum,string>( GL_FLOAT_VEC4,  "GL_FLOAT_VEC4"),
        pair<GLenum,string>( GL_FLOAT_MAT2,  "GL_FLOAT_MAT2"),
        pair<GLenum,string>( GL_FLOAT_MAT3,  "GL_FLOAT_MAT3"),
        pair<GLenum,string>( GL_FLOAT_MAT4,  "GL_FLOAT_MAT4"),
        pair<GLenum,string>( GL_FLOAT_MAT2x3,  "GL_FLOAT_MAT2x3"),
        pair<GLenum,string>( GL_FLOAT_MAT2x4,  "GL_FLOAT_MAT2x4"),
        pair<GLenum,string>( GL_FLOAT_MAT3x2,  "GL_FLOAT_MAT3x2"),
        pair<GLenum,string>( GL_FLOAT_MAT3x4,  "GL_FLOAT_MAT3x4"),
        pair<GLenum,string>( GL_FLOAT_MAT4x2,  "GL_FLOAT_MAT4x2"),
        pair<GLenum,string>( GL_FLOAT_MAT4x3,  "GL_FLOAT_MAT4x3"),
        pair<GLenum,string>( GL_INT,  "GL_INT"),
        pair<GLenum,string>( GL_INT_VEC2,  "GL_INT_VEC2"),
        pair<GLenum,string>( GL_INT_VEC3,  "GL_INT_VEC3"),
        pair<GLenum,string>( GL_INT_VEC4,  "GL_INT_VEC4"),
        pair<GLenum,string>( GL_UNSIGNED_INT,  "GL_UNSIGNED_INT"),
        pair<GLenum,string>( GL_UNSIGNED_INT_VEC2,  "GL_UNSIGNED_INT_VEC2"),
        pair<GLenum,string>( GL_UNSIGNED_INT_VEC3,  "GL_UNSIGNED_INT_VEC3"),
        pair<GLenum,string>( GL_UNSIGNED_INT_VEC4,  "GL_UNSIGNED_INT_VEC4"),
        pair<GLenum,string>( GL_NO_ERROR,  "GL_NO_ERROR"),
        pair<GLenum,string>( GL_INVALID_ENUM,  "GL_INVALID_ENUM"),
        pair<GLenum,string>( GL_INVALID_VALUE,  "GL_INVALID_VALUE"),
        pair<GLenum,string>( GL_INVALID_OPERATION,  "GL_INVALID_OPERATION"),
        pair<GLenum,string>( GL_INVALID_FRAMEBUFFER_OPERATION,  "GL_INVALID_FRAMEBUFFER_OPERATION"),
        pair<GLenum,string>( GL_OUT_OF_MEMORY,  "GL_OUT_OF_MEMORY"),
        /*
        pair<GLenum,string>( GL_STACK_UNDERFLOW,  "GL_STACK_UNDERFLOW"),
        pair<GLenum,string>( GL_STACK_OVERFLOW,  "GL_STACK_OVERFLOW"),
        pair<GLenum,string>( DOUBLE,  "DOUBLE"),
        pair<GLenum,string>( DOUBLE_VEC2,  "DOUBLE_VEC2"),
        pair<GLenum,string>( DOUBLE_VEC3,  "DOUBLE_VEC3"),
        pair<GLenum,string>( DOUBLE_VEC4,  "DOUBLE_VEC4"),
        pair<GLenum,string>( DOUBLE_MAT2,  "DOUBLE_MAT2"),
        pair<GLenum,string>( DOUBLE_MAT3,  "DOUBLE_MAT3"),
        pair<GLenum,string>( DOUBLE_MAT4,  "DOUBLE_MAT4"),
        pair<GLenum,string>( DOUBLE_MAT2x3,  "DOUBLE_MAT2x3"),
        pair<GLenum,string>( DOUBLE_MAT2x4,  "DOUBLE_MAT2x4"),
        pair<GLenum,string>( DOUBLE_MAT3x2,  "DOUBLE_MAT3x2"),
        pair<GLenum,string>( DOUBLE_MAT3x4,  "DOUBLE_MAT3x4"),
        pair<GLenum,string>( DOUBLE_MAT4x2,  "DOUBLE_MAT4x2"),
        pair<GLenum,string>( DOUBLE_MAT4x3,  "DOUBLE_MAT4x3")
        */
      };

    int count = sizeof(pairs)/sizeof(pairs[0]);
    pair<GLenum,string> p;
    for(int i = 0; i < count; ++i){
      p = pairs[i]; 
      enumToString[p.first] = p.second;
      stringToEnum[p.second] = p.first;
    }

  }
}
