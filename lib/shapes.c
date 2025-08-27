#include "./shapes.h"
#include <assert.h>

int global_id = 0;

Sphere sphere(void) {
  return (Sphere){
      .id = ++global_id,
      .transformation = m4_identity(),
      .material = material(),
  };
}

void set_material(Sphere *sphere, MaterialPhong material) {
  sphere->material = material;
}

void set_transform(Sphere *sphere, Mat4 transformation) {
  sphere->transformation = transformation;
}

Vector normal_at(Sphere s, Point world_point) {
  assert(is_point(world_point));
  Point object_point = m4_tmul(m4_inverse(s.transformation), world_point);
  Vector object_normal = tuple_sub(object_point, point(0, 0, 0));
  Vector world_normal =
      m4_tmul(m4_transpose(m4_inverse(s.transformation)), object_normal);
  world_normal.w = 0;
  return tuple_normalize(world_normal);
}
