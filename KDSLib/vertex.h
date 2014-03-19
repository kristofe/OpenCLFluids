#ifndef VERTEX_H
#define VERTEX_H
#include "glutil.h"
#include <string>
#include <vector>

namespace kdslib{

struct VertexAttributeInfo
{
  public:
    VertexAttributeInfo(const std::string pname,
                        int pbuffer_offset,
                        int pmemory_type,
                        int pcount_of_memory_type
                        ) :
                          name(pname),
                          buffer_offset(pbuffer_offset),
                          memory_type(pmemory_type),
                          count_of_memory_type(pcount_of_memory_type),
                          location(0)
    {
    }

  public:
    std::string name;
    int buffer_offset;
    int memory_type;
    int count_of_memory_type;
    int location;
};


struct Vertex
{
public:
  static int get_position_offset(){ return BUFFER_OFFSET(0); }
  static int get_normal_offset(){ return BUFFER_OFFSET(sizeof(Vector3)*1); }
  static int get_uv_offset(){ return BUFFER_OFFSET(sizeof(Vector3)*2); }
  static int get_uv2_offset(){ return BUFFER_OFFSET(sizeof(Vector3)*3); }
  static int get_color_offset(){ return BUFFER_OFFSET(sizeof(Vector3)*4); }

  static void GetAttributeInfo(std::vector<VertexAttributeInfo> *attributes)
  {
      attributes->push_back(VertexAttributeInfo("position",
                                                get_position_offset(),
                                                GL_FLOAT,
                                                3)
                            );

      attributes->push_back(VertexAttributeInfo("normal",
                                                 get_normal_offset(),
                                                 GL_FLOAT,
                                                 3)
                            );

      attributes->push_back(VertexAttributeInfo("uv",
                                                 get_uv_offset(),
                                                 GL_FLOAT,
                                                 3)
                            );

      attributes->push_back(VertexAttributeInfo("uv2",
                                                 get_uv2_offset(),
                                                 GL_FLOAT,
                                                 3)
                            );

      attributes->push_back(VertexAttributeInfo("color",
                                                 get_color_offset(),
                                                 GL_FLOAT,
                                                 3)
                            );
  }

private:
  DISALLOW_COPY_AND_ASSIGN(Vertex);

//Data Members
public:
  Vector3 position;
  Vector3 normal;
  Vector3 uv;
  Vector3 uv2;
  Color   color;
};


}

#endif // VERTEX_H
