#include "./shading.h"
#include <assert.h>

Vector normal_at(Sphere s, Point world_point) {
  assert(is_point(world_point));
  Point object_point = m4_tmul(m4_inverse(s.transformation), world_point);
  Vector object_normal = tuple_sub(object_point, point(0, 0, 0));
  Vector world_normal =
      m4_tmul(m4_transpose(m4_inverse(s.transformation)), object_normal);
  world_normal.w = 0;
  return tuple_normalize(world_normal);
}

Vector reflect(Vector v, Vector n) {
  assert(is_vector(v) && is_vector(n));
  return tuple_sub(v, tuple_smul(tuple_smul(n, 2.0f), tuple_dot_product(v, n)));
}

PointLight pointlight(Point position, Color intensity) {
  assert(is_point(position));
  return (PointLight){position, intensity};
}

Color lighting(MaterialPhong material, Point pos, PointLight light, Vector eye,
               Vector normal) {
  assert(is_point(pos) && is_vector(eye) && is_vector(normal));
  Color color = {0};
  return color;
}
