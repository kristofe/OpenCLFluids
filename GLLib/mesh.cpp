#include "mesh.h"

MeshData::MeshData(std::string name, GLenum type){
  _data = NULL;
  _type = type;//Doesn't seem to be used at all
  //_componentType = componentType; //e.g.  GL_FLOAT, GL_FLOAT_VEC3
  //_itemSize = 3; //Assuming vector3's
  _numItems = 0;
  _buffer = NULL;
  _bufferType = GL_ARRAY_BUFFER;
  _bufferUsage = GL_STATIC_DRAW;
  _name = name;
  _stride = 0; //always 0 as we are not interleaving the arrays
  _pointer = 0;//always 0 as we are not interleaving the arrays
  _normalized = false; //not using fixed point data so always false
}

void MeshData::constructBuffer(void * data, int numItems) {
  _data = data;
  //_itemSize = numComponents;
  _numItems = numItems;
  glGenBuffers(1, &_buffer);
  glBindBuffer( _bufferType, _buffer);

  //var arr = createFloatArray(_data,_itemSize);
  //glBufferData( _bufferType, arr, _bufferUsage );

  //TODO: Check that this _data array can be passed directly
  //In webgl it was converted to flat array of floats

  int numBytes = _numItems * BYTES_PER_FLOAT;
  glBufferData( _bufferType, numBytes, _data, _bufferUsage );

}

void MeshData::updateBuffer()  {
  glBindBuffer( _bufferType, _buffer);
               
  //var arr = _createFloatArray(_data,_itemSize);
  //glBufferSubData(_bufferType, 0, arr);
  glBufferSubData(_bufferType, 0, _data);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
Mesh::Mesh(){

  _attributeMap["Positions"] = {"aPosition",GL_FLOAT_VEC3, 3};
  _attributeMap["Normals"] = {"aNormal",GL_FLOAT_VEC3, 3};
  _attributeMap["UVs"] = {"aUV",GL_FLOAT_VEC4, 4};
  _attributeMap["UV2s"] = {"aUV",GL_FLOAT_VEC2, 2};
  _attributeMap["Tangents"] = {"aTangent",GL_FLOAT_VEC4, 4};
  _attributeMap["Colors"] = {"aColor",GL_FLOAT_VEC4, 4};
}

Mesh::~Mesh(){

}

void Mesh::clear(){
  //_vertices.clear();
  _indices.clear();
  _positions.clear();
  _normals.clear();
  _uvs.clear();
  _uv2s.clear();
  _tangents.clear();
  _colors.clear();
}

void Mesh::constructBuffers() {
  addBuffer(
            constructBuffer(
                              _positions,
                              _positions.size(),
                              _attributeMap["Positions"].name,
                              _attributeMap["Positions"].type
                            )
            );
  
  addBuffer(
            constructBuffer(
                              _normals,
                              _normals.size(),
                              _attributeMap["Normals"].name,
                              _attributeMap["Normals"].type
                            )
            );
  
  addBuffer(
            constructBuffer(
                              _uvs,
                              _uvs.size(),
                              _attributeMap["UVs"].name,
                              _attributeMap["UVs"].type,
                            )
            );

  addBuffer(
            constructBuffer(
                              _uv2s,
                              _uv2s.size(),
                              _attributeMap["UV2s"].name,
                              _attributeMap["UV2s"].type
                            )
            );

  addBuffer(
            constructBuffer(
                              _tangents,
                              _tangents.size(),
                              _attributeMap["Tangents"].name,
                              _attributeMap["Tangents"].type
                            )
            );

  addBuffer(
            constructBuffer(
                              _colors,
                              _colors.size(),
                              _attributeMap["Colors"].name,
                              _attributeMap["Colors"].type
                            )
            );



  _numItems = _positions.size();

}

//TODO: Figure out the types!
MeshData* Mesh::constructBuffer(void * data, int itemCount, std::string& name, GLenum bufferType) {
  if(data == NULL){
    return NULL;
  }
  MeshData* meshData = new MeshData(name,bufferType);
  meshData.constructBuffer(data, itemCount);

  return meshData;
}

void Mesh::addBuffer(MeshData* buf) {
  if(buf != null){
    _buffers.push_back(buf);
  }
}

void Mesh::updateBuffers() {
  for(int i = 0; i < _buffers.size(); i++){
    MeshData* meshData = _buffers[i];
    meshData->updateBuffer();
  }
}


void Mesh::createScreenQuad(Vector2& min, Vector2& max){
  glm::vec3 norm(0,0,1);
  glm::vec3 p0( min.x, min.y, 0.0);
  glm::vec3 p1( max.x, min.y, 0.0);
  glm::vec3 p2( min.x, max.y, 0.0);
  glm::vec3 p3( min.x, max.y, 0.0);
  glm::vec3 p4( max.x, min.y, 0.0);
  glm::vec3 p5( max.x, max.y, 0.0);

  glm::vec2 uv0( 0.0, 0.0);
  glm::vec2 uv1( 1.0, 0.0);
  glm::vec2 uv2( 0.0, 1.0);
  glm::vec2 uv3( 0.0, 1.0);
  glm::vec2 uv4( 1.0, 0.0);
  glm::vec2 uv5( 1.0, 1.0);

  //First Triangle
  _positions.push_back(p0);
  _positions.push_back(p1);
  _positions.push_back(p2);

  _normals.push_back(norm);
  _normals.push_back(norm);
  _normals.push_back(norm);

  _uvs.push_back(uv0);
  _uvs.push_back(uv1);
  _uvs.push_back(uv2);
  
  //Second Triangle
  _positions.push_back(p3);
  _positions.push_back(p4);
  _positions.push_back(p5);

  _normals.push_back(norm);
  _normals.push_back(norm);
  _normals.push_back(norm);

  _uvs.push_back(uv3);
  _uvs.push_back(uv4);
  _uvs.push_back(uv5);


  _primitiveType = GL_TRIANGLES;
  constructBuffers();
}

void Mesh::createSphereMesh(int slices, int stacks){
  createSphereMeshData(slices,stacks);
}

void Mesh::createSphereMeshData(int slices, int stacks){
  for (int stack = 0; stack < stacks; stack++) {
    float theta = stack *  glm::pi() / stacks;
    float thetaNext = (stack + 1) * glm::pi() / stacks;
    float sinTheta = sinf(theta);
    float cosTheta = cosf(theta);
    float sinThetaNext = sinf(thetaNext);
    float cosThetaNext = cosf(thetaNext);


    for (int slice = 0; slice < slices; slice++) {
      flaot phi = slice * 2 * glm::pi() / slices;
      float phiNext = (slice + 1) * 2 * glm::pi() / slices;
      float sinPhi = sinf(phi);
      float cosPhi = cosf(phi);
      float sinPhiNext = sinf(phiNext);
      float cosPhiNext = cosf(phiNext);

      float xTop = cosPhi * sinTheta;
      float xTopNext = cosPhiNext * sinTheta;
      float xBottom = cosPhi * sinThetaNext;
      float xBottomNext = cosPhiNext * sinThetaNext;

      float yTop = cosTheta;
      float yBottom = cosThetaNext;

      float zTop = sinPhi * sinTheta;
      float zTopNext = sinPhiNext * sinTheta;
      float zBottom = sinPhi * sinThetaNext;
      float zBottomNext = sinPhiNext * sinThetaNext;


      float u = 1 - (slice / slices);
      float uNext = 1 - ((slice + 1)/ slices);
      float v = 1 - (stack / stacks);
      float vNext = 1 - ((stack + 1) / stacks);


      glm::vec3 topPos(xTop, yTop, zTop);
      glm::vec3 topNextPos(xTopNext, yTop, zTopNext);
      glm::vec3 bottomPos(xBottom, yBottom, zBottom);
      glm::vec3 bottomNextPos(xBottomNext, yBottom, zBottomNext);

      glm::vec2 topUV(u,v);
      glm::vec2 topNextUV(uNext,v);
      glm::vec2 bottomUV(u,vNext);
      glm::vec2 bottomNextUV(uNext,vNext);

      topPos = glm::normalize(topPos);
      topNextPos = glm::normalize(topNextPos);
      bottomPos = glm::normalize(bottomPos);
      bottomNextPos = glm::normalize(bottomNextPos);

      glm::vec4 topTan = glm::cross(glm::normalize(topPos - bottomPos),topPos);
      glm::vec4 topNextTan = glm::cross(glm::normalize(topNextPos - bottomNextPos),topNextPos);
      glm::vec4 bottomTan = glm::cross(glm::normalize(topPos - bottomPos),bottomPos);
      glm::vec4 bottomNextTan = glm::cross(glm::normalize(topNextPos - bottomNextPos),bottomNextPos);

      //First Triangle
      _positions.push(topPos);
      _positions.push(topNextPos);
      _positions.push(bottomPos);

      _normals.push(topPos);
      _normals.push(topNextPos);
      _normals.push(bottomPos);

      _uvs.push(topUV);
      _uvs.push(topNextUV);
      _uvs.push(bottomUV);

      _tangents.push(topTan);
      _tangents.push(topNextTan);
      _tangents.push(bottomTan);
      
      //Second Triangle
      _positions.push(topNextPos);
      _positions.push(bottomNextPos);
      _positions.push(bottomPos);

      _normals.push(topNextPos);
      _normals.push(bottomNextPos);
      _normals.push(bottomPos);

      _uvs.push(topNextUV);
      _uvs.push(bottomNextUV);
      _uvs.push(bottomUV);

      _tangents.push(topNextTan);
      _tangents.push(bottomNextTan);
      _tangents.push(bottomTan);

    }
  }
  //this.calculateTangents(vertices);

  _primitiveType = gl.TRIANGLES;
  constructBuffers();
}

void Mesh::createGridMesh(int n, int m, bool tileUVs){
 this.createTriStripGridMeshData(n,m);
 //this.createGridMeshData(n,m);
}

void Mesh::createTriStripGridMeshData(int n, int m, bool tileUVs){
  int numVerts = n * m;
  //var vertices = [];

  float xpos = 0.0f;
  float ypos = 0.0f;
  float zpos = 0.0f;
  float xinc = 1.0f/n;
  float yinc = 1.0f/m;

  float x, y, y2, z, u, v, v2;
  for(int j = 0; j < m; ++j){
    for(int i = 0; i <= n; ++i){
      x = 0.5f-xpos;
      y = 0.5f-ypos; 
      y2 = y + yinc;
      z = zpos;

      u = 0.5f + x;
      v =  0.5f + y - yinc;
      v2 = 0.5f + y2 - yinc;

      
      glm::vec3 normal(0.0, 0.0, 1.0);
      glm::vec3 p1(x,y,z);
      glm::vec3 p2(x,y2,z);
      glm::vec4 uv1(u,v,0);
      glm::vec2 uv2(u,v2,0);
      glm::vec4 tangent(1.0, 0.0, 0.0, 0.0);



      //Part of degenerate triangle
      if(i == 0 && j > 0){
        _positions.push_back(p1);
        _normals.push_back(normal);
        _uvs.push_back(uv1);
        _tangents.push_back(tangent);
      }
      _positions.push_back(p1);
      _normals.push_back(normal);
      _uvs.push_back(uv1);
      _tangents.push_back(tangent);

      _positions.push_back(p2);
      _normals.push_back(normal);
      _uvs.push_back(uv2);
      _tangents.push_back(tangent);
      xpos += xinc;
    }
    //Part of degenerate triangle
    _positions.push_back(p2);
    _normals.push_back(normal);
    _uvs.push_back(uv2);
    _tangents.push_back(tangent);
    //vertices.push(x, y2, z, 0.0, 0.0, 1.0, u, v2, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

    ypos += yinc;
    xpos = 0.0;
  }

  _primitiveType = GL_TRIANGLE_STRIP;
  constructBuffers();
 }



/*
void CalculateTangentArray(long vertexCount, const Point3D *vertex, const Vector3D *normal,
        const Point2D *texcoord, long triangleCount, const Triangle *triangle, Vector4D *tangent)
{
    glm::vec3 *tan1 = new glm::vec3[vertexCount * 2];
    glm::vec3 *tan2 = tan1 + vertexCount;
    ZeroMemory(tan1, vertexCount * sizeof(glm::vec3) * 2);
    
    for (long a = 0; a < triangleCount; a++)
    {
        long i1 = triangle->index[0];
        long i2 = triangle->index[1];
        long i3 = triangle->index[2];
        
        const glm::vec3& v1 = vertex[i1];
        const glm::vec3& v2 = vertex[i2];
        const glm::vec3& v3 = vertex[i3];
        
        const glm::vec2& w1 = texcoord[i1];
        const glm::vec2& w2 = texcoord[i2];
        const glm::vec2& w3 = texcoord[i3];
        
        float x1 = v2.x - v1.x;
        float x2 = v3.x - v1.x;
        float y1 = v2.y - v1.y;
        float y2 = v3.y - v1.y;
        float z1 = v2.z - v1.z;
        float z2 = v3.z - v1.z;
        
        float s1 = w2.x - w1.x;
        float s2 = w3.x - w1.x;
        float t1 = w2.y - w1.y;
        float t2 = w3.y - w1.y;
        
        float r = 1.0F / (s1 * t2 - s2 * t1);
        glm::vec3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
                (t2 * z1 - t1 * z2) * r);
        glm::vec3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
                (s1 * z2 - s2 * z1) * r);
        
        tan1[i1] += sdir;
        tan1[i2] += sdir;
        tan1[i3] += sdir;
        
        tan2[i1] += tdir;
        tan2[i2] += tdir;
        tan2[i3] += tdir;
        
        triangle++;
    }
    
    for (long a = 0; a < vertexCount; a++)
    {
        const glm::vec3& n = normal[a];
        const glm::vec3& t = tan1[a];
        
        // Gram-Schmidt orthogonalize
        tangent[a] = (t - n * Dot(n, t)).Normalize();
        
        // Calculate handedness
        tangent[a].w = (Dot(Cross(n, t), tan2[a]) < 0.0F) ? -1.0F : 1.0F;
    }
    
    delete[] tan1;
}
*/

/*
Mesh.prototype.createSphereMeshDataWithIndices = function(slices, stacks){
  vertices = [];
  for (var stack = 0; stack < stacks; stack++) {
    var theta = stack * glm::pi() / stacks;
    var sinTheta = sinf(theta);
    var cosTheta = cosf(theta);

    for (var slice = 0; slice < slices; slice++) {
      var phi = slice * 2 * glm::pi() / slices;
      var sinPhi = sinf(phi);
      var cosPhi = cosf(phi);

      var x = cosPhi * sinTheta;
      var y = cosTheta;
      var z= sinPhi * sinTheta;

      var u = 1 - (slice / slices);
      var v = 1 - (stack / stacks);

      vertices.push(x,y,z, x,y,z, u,v,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    }
  }

  indices = [];
  for (var slice = 0; slice < slices; slice++) {
    for (var stack = 0; stack < stacks; stack++) {
      var first = (slice * (stacks + 1)) + stack;
      var second = first + stacks + 1;
      indices.push(first);
      indices.push(second);
      indices.push(first + 1);

      indices.push(second);
      indices.push(second + 1);
      indices.push(first + 1);
    }
  }
  return vertices;
}
*/
