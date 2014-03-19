#ifndef VECTOR3_H
#define VECTOR3_H
#include "utils.h"

namespace kdslib{

struct Vector3
{
public:
  Vector3():x(0.0f),y(0.0f),z(0.0f){};
  Vector3(float px, float py, float pz):x(px),y(py),z(pz){};

private:
  DISALLOW_COPY_AND_ASSIGN(Vector3);

public:
  float x,y,z;
};

}

#endif // VECTOR3_H
