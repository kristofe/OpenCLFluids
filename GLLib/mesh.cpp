#include "mesh.h"

MeshData::MeshData(std::string name, GLuint type, GLenum componentType){

  _data = NULL;
  _type = type;
  _componentType = componentType; //e.g.  GL_FLOAT, GL_FLOAT_VEC3
  _itemSize = 3; //Assuming vector3's
  _numItems = 0;
  _buffer = NULL;
  _bufferType = GL_ARRAY_BUFFER;
  _bufferUsage = GL_STATIC_DRAW;
  _name = name;
  _stride = 0; //always 0 as we are not interleaving the arrays
  _pointer = 0;//always 0 as we are not interleaving the arrays
  _normalized = false; //not using fixed point data so always false
}

void MeshData::constructBuffer(void * data, int arrayLength, int numComponents) {
  _data = data;
  _itemSize = numComponents;
  _numItems = data.length;
  glGenBuffers(1, &_buffer);
  glBindBuffer( _bufferType, _buffer);

  //var arr = createFloatArray(_data,_itemSize);
  //glBufferData( _bufferType, arr, _bufferUsage );

  //TODO: Check that this _data array can be passed directly
  //In webgl it was converted to flat array of floats
  glBufferData( _bufferType, _data, _bufferUsage );

}

void MeshData::updateBuffer()  {
  glBindBuffer( _bufferType, _buffer);
               
  //var arr = _createFloatArray(_data,_itemSize);
  //glBufferSubData(_bufferType, 0, arr);
  glBufferSubData(_bufferType, 0, _data);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
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
                              _attributeMap["Positions"].name,
                              _attributeMap["Positions"].type,
                              _attributeMap["Positions"].numComponents
                            )
            );
  
  addBuffer(
            constructBuffer(
                              _normals,
                              _attributeMap["Normals"].name,
                              _attributeMap["Normals"].type,
                              _attributeMap["Normals"].numComponents
                            )
            );
  
  addBuffer(
            constructBuffer(
                              _uvs,
                              _attributeMap["UVs"].name,
                              _attributeMap["UVs"].type,
                              _attributeMap["UVs"].numComponents
                            )
            );

  addBuffer(
            constructBuffer(
                              _uv2s,
                              _attributeMap["UV2s"].name,
                              _attributeMap["UV2s"].type,
                              _attributeMap["UV2s"].numComponents
                            )
            );

  addBuffer(
            constructBuffer(
                              _tangents,
                              _attributeMap["Tangents"].name,
                              _attributeMap["Tangents"].type,
                              _attributeMap["Tangents"].numComponents
                            )
            );

  addBuffer(
            constructBuffer(
                              _colors,
                              _attributeMap["Colors"].name,
                              _attributeMap["Colors"].type,
                              _attributeMap["Colors"].numComponents
                            )
            );



  _numItems = _positions.size();

}

//TODO: Figure out the types!
MeshData& Mesh::constructBuffer(data, name, type, numComponents) {
  if(data.length == 0){
    return NULL;
  }
  var meshData = new MeshData(this.gl,name,type);
  meshData.constructBuffer(data, numComponents);

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
  Vector3 norm(0,0,1);
  Vector3 p0( min.x, min.y, 0.0);
  Vector3 p1( max.x, min.y, 0.0);
  Vector3 p2( min.x, max.y, 0.0);
  Vector3 p3( min.x, max.y, 0.0);
  Vector3 p4( max.x, min.y, 0.0);
  Vector3 p5( max.x, max.y, 0.0);

  Vector2 uv0( 0.0, 0.0);
  Vector2 uv1( 1.0, 0.0);
  Vector2 uv2( 0.0, 1.0);
  Vector2 uv3( 0.0, 1.0);
  Vector2 uv4( 1.0, 0.0);
  Vector2 uv5( 1.0, 1.0);

  //First Triangle
  _positions.push_back(p0, p1, p2);
  _normals.push_back(norm, norm, norm);
  _uvs.push_back(uv0, uv1, uv2);
  
  //Second Triangle
  _positions.push_back(p3, p4, p5);
  _normals.push_back(norm, norm, norm);
  _uvs.push_back(uv3, uv4, uv5);


  _primitiveType = GL_TRIANGLES;
  constructBuffers();
}

void Mesh::createSphereMesh(int slices, int stacks){
  createSphereMeshData(slices,stacks);
}

void Mesh::createSphereMeshData(int slices, int stacks){
  for (var stack = 0; stack < stacks; stack++) {
    var theta = stack * Math.PI / stacks;
    var thetaNext = (stack + 1) * Math.PI / stacks;
    var sinTheta = Math.sin(theta);
    var cosTheta = Math.cos(theta);
    var sinThetaNext = Math.sin(thetaNext);
    var cosThetaNext = Math.cos(thetaNext);


    for (var slice = 0; slice < slices; slice++) {
      var phi = slice * 2 * Math.PI / slices;
      var phiNext = (slice + 1) * 2 * Math.PI / slices;
      var sinPhi = Math.sin(phi);
      var cosPhi = Math.cos(phi);
      var sinPhiNext = Math.sin(phiNext);
      var cosPhiNext = Math.cos(phiNext);

      var xTop = cosPhi * sinTheta;
      var xTopNext = cosPhiNext * sinTheta;
      var xBottom = cosPhi * sinThetaNext;
      var xBottomNext = cosPhiNext * sinThetaNext;

      var yTop = cosTheta;
      var yBottom = cosThetaNext;

      var zTop = sinPhi * sinTheta;
      var zTopNext = sinPhiNext * sinTheta;
      var zBottom = sinPhi * sinThetaNext;
      var zBottomNext = sinPhiNext * sinThetaNext;


      var u = 1 - (slice / slices);
      var uNext = 1 - ((slice + 1)/ slices);
      var v = 1 - (stack / stacks);
      var vNext = 1 - ((stack + 1) / stacks);


      var topPos = new Vector3(xTop, yTop, zTop);
      var topNextPos = new Vector3(xTopNext, yTop, zTopNext);
      var bottomPos = new Vector3(xBottom, yBottom, zBottom);
      var bottomNextPos = new Vector3(xBottomNext, yBottom, zBottomNext);

      var topUV = new Vector2(u,v);
      var topNextUV = new Vector2(uNext,v);
      var bottomUV = new Vector2(u,vNext);
      var bottomNextUV = new Vector2(uNext,vNext);

      topPos.normalize();
      topNextPos.normalize();
      bottomPos.normalize();
      bottomNextPos.normalize();

      var topTan = topPos.clone().subtract(bottomPos).normalize().cross(topPos);
      var topNextTan = topNextPos.clone().subtract(bottomNextPos).normalize().cross(topNextPos);
      var bottomTan = topPos.clone().subtract(bottomPos).normalize().cross(bottomPos);
      var bottomNextTan = topNextPos.clone().subtract(bottomNextPos).normalize().cross(bottomNextPos);

      //First Triangle
      this.positions.push(topPos,topNextPos,bottomPos);
      this.normals.push(topPos,topNextPos,bottomPos);
      this.uvs.push(topUV,topNextUV,bottomUV);
      this.tangents.push(topTan,topNextTan,bottomTan);
      
      //Second Triangle
      this.positions.push(topNextPos,bottomNextPos,bottomPos);
      this.normals.push(topNextPos,bottomNextPos,bottomPos);
      this.uvs.push(topNextUV,bottomNextUV,bottomUV);
      this.tangents.push(topNextTan,bottomNextTan,bottomTan);

    }
  }
  //this.calculateTangents(vertices);

  this.primitiveType = gl.TRIANGLES;
  this.constructBuffers();
}

void Mesh::createGridMesh(n, m, tileUVs){
 this.createTriStripGridMeshData(n,m);
 //this.createGridMeshData(n,m);
}

void Mesh::createTriStripGridMeshData(n, m, tileUVs){
  var numVerts = n * m;
  var vertices = [];

  var xpos = 0.0;
  var ypos = 0.0;
  var zpos = 0.0;
  var xinc = 1/n;
  var yinc = 1/m;

  var x, y, y2, z, u, v, v2;
  for(var j = 0; j < m; ++j){
    for(var i = 0; i <= n; ++i){
      x = 0.5-xpos;
      y = 0.5-ypos; 
      y2 = y + yinc;
      z = zpos;

      u = 0.5 + x;
      v =  0.5 + y - yinc;
      v2 = 0.5 + y2 - yinc;

      
      var normal = new Vector3(0.0, 0.0, 1.0);
      var p1 = new Vector3(x,y,z);
      var p2 = new Vector3(x,y2,z);
      var uv1 = new Vector3(u,v,0);
      var uv2 = new Vector3(u,v2,0);
      var tangent = new Vector4(1.0, 0.0, 0.0, 0.0);



      //Part of degenerate triangle
      if(i == 0 && j > 0){
        this.positions.push(p1);
        this.normals.push(normal);
        this.uvs.push(uv1);
        this.tangents.push(tangent);
      }
      this.positions.push(p1);
      this.normals.push(normal);
      this.uvs.push(uv1);
      this.tangents.push(tangent);

      this.positions.push(p2);
      this.normals.push(normal);
      this.uvs.push(uv2);
      this.tangents.push(tangent);
      xpos += xinc;
    }
    //Part of degenerate triangle
    this.positions.push(p2);
    this.normals.push(normal);
    this.uvs.push(uv2);
    this.tangents.push(tangent);
    vertices.push(x, y2, z, 0.0, 0.0, 1.0, u, v2, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0,
        0.0, 0.0);

    ypos += yinc;
    xpos = 0.0;
  }

  this.primitiveType = gl.TRIANGLE_STRIP;
  this.constructBuffers();
 }

void* Mesh::createGridMeshData(n, m, tileUVs){
  var numVerts = n * m;
  var vertices = [];

  var xpos = 0.0;
  var ypos = 0.0;
  var zpos = 0.0;
  var xinc = 1/n;
  var yinc = 1/m;
  for(var j = 0; j < m; ++j){
    for(var i = 0; i < n; ++i){
      var lx = 0.5-xpos;
      var rx = 0.5-xpos + xinc;
      var dy = 0.5-ypos; 
      var uy = 0.5-ypos + yinc;

      if(tileUVs == false){
        var uv1 = xpos;
        var uvr = xpos + xinc;
        var uvd = ypos; 
        var uvu = ypos + yinc;
      }
      else
      {
        var uvl = i;
        var uvr = i+1;
        var uvd = j+1;
        var uvu = j;
      }

      
      vertices.push(lx, dy, zpos, 0.0, 1.0, 0.0, uvl, uvd, 1.0, 0.0, 0.0, 0.0,
          0.0, 0.0, 0.0, 0.0);
      vertices.push(rx, dy, zpos, 0.0, 1.0, 0.0, uvr, uvd, 1.0, 0.0, 0.0, 0.0,
          0.0, 0.0, 0.0, 0.0);
      vertices.push(rx, uy, zpos, 0.0, 1.0, 0.0, uvr, uvu, 1.0, 0.0, 0.0, 0.0,
          0.0, 0.0, 0.0, 0.0);

      vertices.push(lx, dy, zpos, 0.0, 1, 0.0, uvl, uvd, 1.0, 0.0, 0.0, 0.0, 
          0.0, 0.0, 0.0, 0.0);
      vertices.push(rx, uy, zpos, 0.0, 1, 0.0, uvr, uvu, 1.0, 0.0, 0.0, 0.0, 
          0.0, 0.0, 0.0, 0.0);
      vertices.push(lx, uy, zpos, 0.0, 1, 0.0, uvl, uvu, 1.0, 0.0, 0.0, 0.0, 
          0.0, 0.0, 0.0, 0.0);
      xpos += xinc;
    }
    ypos += yinc;
    xpos = 0.0;
  }

  return vertices;
}



void Mesh::calculateTangents(verts)
{
  if(this.primitiveType != this.gl.TRIANGLES){
    console.error("Can't create tangents for mesh that isn't GL_TRIANGLES");
    return;
  }

  var vertexCount = verts.length / this.stride; 
  var triCount = vertexCount / 3; //Assuming gl.TRIANGLES

  var tan1 = new Float32Array(vertexCount);
  var tan2 = new Float32Array(vertexCount);

  for(var triID = 0; triID < triCount; triID++){
    var i1 = triID * this.stride * 3;
    var i2 =  i1 + this.stride;
    var i3 =  i2 + this.stride;

    //pos(3) normal(3) UV1(3) UV2(3) COLOR(4)
    var v1 = new Vector3(verts[i1], verts[i1+1], verts[i1 + 2]);  
    var w1 = new Vector2(verts[i1 + 6], verts[i1+7]);

    var v2 = new Vector3(verts[i2], verts[i2+1], verts[i2 + 2]);  
    var w2 = new Vector2(verts[i2 + 6], verts[i2+7]);

    var v3 = new Vector3(verts[i3], verts[i3+1], verts[i3 + 2]);  
    var w3 = new Vector2(verts[i3 + 6], verts[i3+7]);

    var x1 = v2.x - v1.x;
    var x2 = v3.x - v1.x;
    var y1 = v2.y - v1.y;
    var y2 = v3.y - v1.y;
    var z1 = v2.z - v1.z;
    var z2 = v3.z - v1.z;
    
    var s1 = w2.x - w1.x;
    var s2 = w3.x - w1.x;
    var t1 = w2.y - w1.y;
    var t2 = w3.y - w1.y;
    
    var r = 1.0 / (s1 * t2 - s2 * t1);
    var sdir = new Vector3((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
            (t2 * z1 - t1 * z2) * r);
    var tdir = new Vector3((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
            (s1 * z2 - s2 * z1) * r);
    
    tan1[i1/this.stride] += sdir;
    tan1[i2/this.stride] += sdir;
    tan1[i3/this.stride] += sdir;
    
    tan2[i1/this.stride] += tdir;
    tan2[i2/this.stride] += tdir;
    tan2[i3/this.stride] += tdir;

  }

   for (var i = 0; i < vertexCount; i++)
    {
      var a = i * this.stride;
      var tidx = i * 3;//Stride in tan arrays is 3

      //pos(3) normal(3) UV1(3) UV2(3) COLOR(4)
      var n = new Vector3(verts[a], verts[a+1], verts[a+2]);

      var t1 = new Vector3(tan1[tidx], tan1[tidx+1], tan1[tidx+2]);
      var t2 = new Vector3(tan2[tidx], tan2[tidx+1], tan2[tidx+2]);
        
      // Gram-Schmidt orthogonalize
      var tangent = new Vector4(0,0,0,0);
      var dnt1 = Vector3.dot(n, t1);
      var tangent = t1.subtract(n).multiply(dnt1).normalize();

      //tangent[a] = (t - n * Dot(n, t)).Normalize();
      
      // Calculate handedness
      tangent.w = (Vector3.dot(Vector3.cross(n, t1), t2) < 0.0) ? -1.0 : 1.0;

      verts[i + 9] = tangent.x;
      verts[i + 10] = tangent.y;
      verts[i + 11] = tangent.z;

      verts[i + 12] = tangent.x;
      verts[i + 13] = tangent.y;
      verts[i + 14] = tangent.z;
      verts[i + 15] = tangent.w;
    }
}


/*

void CalculateTangentArray(long vertexCount, const Point3D *vertex, const Vector3D *normal,
        const Point2D *texcoord, long triangleCount, const Triangle *triangle, Vector4D *tangent)
{
    Vector3D *tan1 = new Vector3D[vertexCount * 2];
    Vector3D *tan2 = tan1 + vertexCount;
    ZeroMemory(tan1, vertexCount * sizeof(Vector3D) * 2);
    
    for (long a = 0; a < triangleCount; a++)
    {
        long i1 = triangle->index[0];
        long i2 = triangle->index[1];
        long i3 = triangle->index[2];
        
        const Point3D& v1 = vertex[i1];
        const Point3D& v2 = vertex[i2];
        const Point3D& v3 = vertex[i3];
        
        const Point2D& w1 = texcoord[i1];
        const Point2D& w2 = texcoord[i2];
        const Point2D& w3 = texcoord[i3];
        
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
        Vector3D sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
                (t2 * z1 - t1 * z2) * r);
        Vector3D tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
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
        const Vector3D& n = normal[a];
        const Vector3D& t = tan1[a];
        
        // Gram-Schmidt orthogonalize
        tangent[a] = (t - n * Dot(n, t)).Normalize();
        
        // Calculate handedness
        tangent[a].w = (Dot(Cross(n, t), tan2[a]) < 0.0F) ? -1.0F : 1.0F;
    }
    
    delete[] tan1;
}

Mesh.prototype.createSphereMeshDataWithIndices = function(slices, stacks){
  vertices = [];
  for (var stack = 0; stack < stacks; stack++) {
    var theta = stack * Math.PI / stacks;
    var sinTheta = Math.sin(theta);
    var cosTheta = Math.cos(theta);

    for (var slice = 0; slice < slices; slice++) {
      var phi = slice * 2 * Math.PI / slices;
      var sinPhi = Math.sin(phi);
      var cosPhi = Math.cos(phi);

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
