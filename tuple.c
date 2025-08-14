#include "./tuple.h"
#include "./util.h"
#include <assert.h>
#include <math.h>

Tuple tuple(float x, float y, float z, float w) { return (Tuple){x, y, z, w}; }
Tuple point(float x, float y, float z) { return (Tuple){x, y, z, 1}; }
Tuple vector(float x, float y, float z) { return (Tuple){x, y, z, 0}; }

bool tuple_equal(Tuple a, Tuple b) {
  return near(a.x, b.x) && near(a.y, b.y) && near(a.z, b.z);
}

Tuple tuple_add(Tuple a, Tuple b) {
  return (Tuple){a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}
Tuple tuple_sub(Tuple a, Tuple b) {
  return (Tuple){a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

Tuple tuple_neg(Tuple a) { return (Tuple){-a.x, -a.y, -a.z, a.w}; }

Tuple tuple_smul(Tuple a, float by) {
  return vector(a.x * by, a.y * by, a.z * by);
}
Tuple tuple_sdiv(Tuple a, float by) {
  return vector(a.x / by, a.y / by, a.z / by);
}

float tuple_length(Tuple a) { return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z); }

Tuple tuple_normalize(Tuple a) {
  float length = tuple_length(a);
  return vector(a.x / length, a.y / length, a.z / length);
}

float tuple_dot_product(Tuple a, Tuple b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

Tuple tuple_cross_product(Tuple a, Tuple b) {
  return vector(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x);
}

int main(void) {
  assert(tuple_equal(tuple_add(tuple(3, -2, 5, 1), tuple(-2, 3, 1, 0)),
                     tuple(1, 1, 6, 1)));
  assert(tuple_equal(tuple_sub(point(3, 2, 1), point(5, 6, 7)),
                     vector(-2, -4, -6)));
  assert(tuple_equal(tuple_sub(point(3, 2, 1), vector(5, 6, 7)),
                     point(-2, -4, -6)));
  assert(tuple_equal(tuple_sub(vector(3, 2, 1), vector(5, 6, 7)),
                     vector(-2, -4, -6)));
  assert(tuple_equal(tuple_neg(tuple(1, -2, 3, -4)), tuple(-1, 2, -3, 4)));
  assert(tuple_equal(tuple_smul(tuple(1, -2, 3, -4), 3.5),
                     tuple(3.5, -7, 10.5, -14)));
  assert(tuple_equal(tuple_smul(tuple(1, -2, 3, -4), 0.5),
                     tuple(0.5, -1, 1.5, -2)));
  assert(
      tuple_equal(tuple_sdiv(tuple(1, -2, 3, -4), 2), tuple(0.5, -1, 1.5, -2)));
  assert(1 == tuple_length(vector(1, 0, 0)));
  assert(1 == tuple_length(vector(0, 1, 0)));
  assert(1 == tuple_length(vector(0, 0, 1)));
  assert(sqrtf(14) == tuple_length(vector(1, 2, 3)));
  assert(sqrtf(14) == tuple_length(vector(-1, -2, -3)));
  assert(tuple_equal(tuple_normalize(vector(4, 0, 0)), vector(1, 0, 0)));
  assert(tuple_equal(tuple_normalize(vector(1, 2, 3)),
                     vector(0.26726, 0.53452, 0.80178)));
  assert(near(1, tuple_length(tuple_normalize(vector(1, 2, 3)))));
  assert(near(20, tuple_dot_product(vector(1, 2, 3), vector(2, 3, 4))));
  assert(tuple_equal(tuple_cross_product(vector(1, 2, 3), vector(2, 3, 4)),
                     vector(-1, 2, -1)));
  assert(tuple_equal(tuple_cross_product(vector(2, 3, 4), vector(1, 2, 3)),
                     vector(1, -2, 1)));
  return 0;
}
