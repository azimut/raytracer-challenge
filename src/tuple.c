#include "./tuple.h"
#include "./util.h"
#include <math.h>

Tuple tuple(float x, float y, float z, float w) { return (Tuple){x, y, z, w}; }
Tuple point(float x, float y, float z) { return (Tuple){x, y, z, 1}; }
Tuple vector(float x, float y, float z) { return (Tuple){x, y, z, 0}; }

bool is_point(Tuple t) { return t.w == 1; };
bool is_vector(Tuple t) { return t.w == 0; };

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
