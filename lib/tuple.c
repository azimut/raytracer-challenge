#include "./tuple.h"
#include "./util.h"
#include <math.h>

Tuple tuple(double x, double y, double z, double w) {
  return (Tuple){x, y, z, w};
}

bool is_point(const Tuple t) { return t.w == 1; }
bool is_vector(const Tuple t) { return t.w == 0; }

bool tuple_equal(const Tuple a, const Tuple b) {
  return near(a.x, b.x) && near(a.y, b.y) && near(a.z, b.z);
}

Tuple tuple_add(const Tuple a, const Tuple b) {
  return (Tuple){a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}
Tuple tuple_sub(const Tuple a, const Tuple b) {
  return (Tuple){a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

Tuple tuple_neg(const Tuple a) { return (Tuple){-a.x, -a.y, -a.z, a.w}; }

Tuple tuple_smul(const Tuple a, double by) {
  return VECTOR(a.x * by, a.y * by, a.z * by);
}
Tuple tuple_sdiv(const Tuple a, double by) {
  return VECTOR(a.x / by, a.y / by, a.z / by);
}

double tuple_length(const Tuple a) {
  return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

Tuple tuple_normalize(const Tuple a) {
  double length = tuple_length(a);
  return VECTOR(a.x / length, a.y / length, a.z / length);
}

double tuple_dot_product(const Tuple a, const Tuple b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

Tuple tuple_cross_product(const Tuple a, const Tuple b) {
  return VECTOR(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
                a.x * b.y - a.y * b.x);
}
