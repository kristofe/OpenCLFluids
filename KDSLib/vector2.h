#ifndef VECTOR2_H
#define VECTOR2_H
#include "utils.h"

namespace kdslib{

struct Vector2
{
public:
  Vector2():x(0.0f),y(0.0f){};
  Vector2(float px, float py):x(px),y(py){};

private:
  DISALLOW_COPY_AND_ASSIGN(Vector2);

public:
  float x,y;
};

}

#endif // VECTOR2_H
