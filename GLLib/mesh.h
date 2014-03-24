#ifndef GLLIB_MESH_H
#define GLLIB_MESH_H

#include "glutils.h"

#define BYTES_PER_FLOAT 4

namespace gllib
{
  class MeshData{

    public:
      MeshData(std::string name, GLenum type);

    public:
      void constructBuffer(void * data, int numItems); 
      void updateBuffer();

    public:
      const GLvoid*   _data;
      GLenum          _type;//Doesn't seem to be used at all
      //GLenum          _componentType;
      //int             _itemSize;
      int             _numItems;
      GLuint          _buffer;
      GLenum          _bufferType;
      GLenum          _bufferUsage;
      std::string     _name;
      int             _stride;
      void*           _pointer;
      bool            _normalized;
  };


struct AttributeData{
  std::string _name;
  GLenum      _type;//GL_FLOAT_VEC3 etc
  int         _numComponents;
};

class Mesh{

  public:
    Mesh();
    ~Mesh();

    void clear();
    void constructBuffers() ;
    MeshData& constructBuffer(void * data, int data_size, std::string& name, GLuint type, int numComponents);
    void addBuffer(MeshData* buf) ;
    void updateBuffers() ;
    void createScreenQuad(Vector2& min, Vector2& max);
    void createSphereMesh(int slices, int stacks);
    void createSphereMeshData(int slices, int stacks);
    void createGridMesh(n, m, tileUVs);
    void createTriStripGridMeshData(n, m, tileUVs);

  public:
    std::vector<MeshData*> _buffers;
    std::map<std::string, AttributeData> _attributeMap;
    /*
    _attributeMap = {};
    _attributeMap["Positions"] = {name: "aPosition", type: "FLOAT_VEC3"};
    _attributeMap["Normals"] = {name:"aNormal", type: "FLOAT_VEC3"};
    _attributeMap["UVs"] = {name:"aUV", type: "FLOAT_VEC2"};
    _attributeMap["UV2s"] = {name:"aUV2", type: "FLOAT_VEC2"};
    _attributeMap["Tangents"] = {name:"aTangent", type: "FLOAT_VEC4"};
    _attributeMap["Colors"] = {name:"aColor", type: "FLOAT_VEC4"};
    */

    //_vertices = []; //I Don't know what type this is going to be!!!
    std::vector<int>      _indices;
    std::vector<Vector3>  _positions;
    std::vector<Vector3>  _normals;
    std::vector<Vector4>  _uvs;
    std::vector<Vector2>  _uv2s;
    std::vector<Vector4>  _tangents;
    std::vector<Vector4>  _colors;


    GLuint _vertexBuffer;
    GLuint _indexBuffer;
    GLenum _primitiveType;// GL_TRIANGLES;
    GLuint _stride;//16
    GLint  _vertCount;

    
    GLuint  _numVertices;

};
    
} //namespace gllib
#endif //#GLIB_MEHS_H
