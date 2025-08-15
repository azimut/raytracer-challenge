#include "./transformation.h"
#include "./tuple.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

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

int main(void) {
  Mat4 t = translation(5, -3, 2);
  assert(tuple_equal(m4_tmul(t, point(-3, 4, 5)), point(2, 1, 7)));
  assert(tuple_equal(m4_tmul(m4_inverse(t), point(-3, 4, 5)), point(-8, 7, 3)));
  assert(tuple_equal(m4_tmul(t, vector(-3, 4, 5)), vector(-3, 4, 5)));

  Mat4 s = scaling(2, 3, 4);
  assert(tuple_equal(m4_tmul(s, point(-4, 6, 8)), point(-8, 18, 32)));
  assert(tuple_equal(m4_tmul(s, vector(-4, 6, 8)), vector(-8, 18, 32)));
  assert(
      tuple_equal(m4_tmul(m4_inverse(s), vector(-4, 6, 8)), vector(-2, 2, 2)));
  assert(
      tuple_equal(m4_tmul(scaling(-1, 1, 1), point(2, 3, 4)), point(-2, 3, 4)));
  // rotation_x()
  assert(tuple_equal(m4_tmul(rotation_x(M_PI / 4), point(0, 1, 0)),
                     point(0, sqrtf(2) / 2, sqrtf(2) / 2)));
  assert(tuple_equal(m4_tmul(rotation_x(M_PI / 2), point(0, 1, 0)),
                     point(0, 0, 1)));
  assert(tuple_equal(m4_tmul(m4_inverse(rotation_x(M_PI / 4)), point(0, 1, 0)),
                     point(0, sqrtf(2) / 2, -sqrtf(2) / 2)));
  // rotation_y()
  assert(tuple_equal(m4_tmul(rotation_y(M_PI / 4), point(0, 0, 1)),
                     point(sqrtf(2) / 2, 0, sqrtf(2) / 2)));
  assert(tuple_equal(m4_tmul(rotation_y(M_PI / 2), point(0, 0, 1)),
                     point(1, 0, 0)));
  // rotation_z()
  assert(tuple_equal(m4_tmul(rotation_z(M_PI / 4), point(0, 1, 0)),
                     point(-sqrtf(2) / 2, sqrtf(2) / 2, 0)));
  assert(tuple_equal(m4_tmul(rotation_z(M_PI / 2), point(0, 1, 0)),
                     point(-1, 0, 0)));
  // shearing()
  assert(tuple_equal(m4_tmul(shearing(0, 1, 0, 0, 0, 0), point(2, 3, 4)),
                     point(6, 3, 4)));
  assert(tuple_equal(m4_tmul(shearing(0, 0, 1, 0, 0, 0), point(2, 3, 4)),
                     point(2, 5, 4)));
  assert(tuple_equal(m4_tmul(shearing(0, 0, 0, 1, 0, 0), point(2, 3, 4)),
                     point(2, 7, 4)));
  assert(tuple_equal(m4_tmul(shearing(0, 0, 0, 0, 1, 0), point(2, 3, 4)),
                     point(2, 3, 6)));
  assert(tuple_equal(m4_tmul(shearing(0, 0, 0, 0, 0, 1), point(2, 3, 4)),
                     point(2, 3, 7)));
  printf("ALL FINE?\n");
  return 0;
}
