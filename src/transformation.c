#include "./transformation.h"
#include "./tuple.h"
#include <math.h>

float radians(float degrees) { return (degrees / 180.0f) * M_PI; }

Mat4 translation(float x, float y, float z) {
  return m4(1, 0, 0, x, 0, 1, 0, y, 0, 0, 1, z, 0, 0, 0, 1);
}

Mat4 rotation_x(float rad) {
  return m4(1, 0, 0, 0, 0, cosf(rad), -sinf(rad), 0, 0, sinf(rad), cosf(rad), 0,
            0, 0, 0, 1);
}

Mat4 rotation_y(float rad) {
  return m4(cosf(rad), 0, sinf(rad), 0, 0, 1, 0, 0, -sinf(rad), 0, cosf(rad), 0,
            0, 0, 0, 1);
}

Mat4 rotation_z(float rad) {
  return m4(cosf(rad), -sinf(rad), 0, 0, sinf(rad), cosf(rad), 0, 0, 0, 0, 1, 0,
            0, 0, 0, 1);
}

Mat4 scaling(float x, float y, float z) {
  return m4(x, 0, 0, 0, 0, y, 0, 0, 0, 0, z, 0, 0, 0, 0, 1);
}

Mat4 shearing(float xy, float xz, float yx, float yz, float zx, float zy) {
  return m4(1, xy, xz, 0, yx, 1, yz, 0, zx, zy, 1, 0, 0, 0, 0, 1);
}
