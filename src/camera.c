#include "./camera.h"
#include "./transformation.h"
#include <assert.h>

Mat4 view_transform(Point from, Point to, Vector up) {
  assert(is_point(from) && is_point(to) && is_vector(up));
  Vector forward = tuple_normalize(tuple_sub(to, from));
  Vector left = tuple_cross_product(forward, tuple_normalize(up));
  Vector true_up = tuple_cross_product(left, forward);
  Mat4 orientation =
      m4(left.x, left.y, left.z, 0, true_up.x, true_up.y, true_up.z, 0,
         -forward.x, -forward.y, -forward.z, 0, 0, 0, 0, 1);
  return m4_mul(orientation, translation(-from.x, -from.y, -from.z));
}
