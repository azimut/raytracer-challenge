#include "./shapes.h"
#include <assert.h>

int global_id = 0;

Shape plane(void) {
  return (Shape){
      .id = ++global_id,
      .transformation = m4_identity(),
      .material = material(),
      .shape_type = SHAPE_TYPE_PLANE,
  };
}

Shape sphere(void) {
  return (Shape){
      .id = ++global_id,
      .transformation = m4_identity(),
      .material = material(),
      .shape_type = SHAPE_TYPE_SPHERE,
  };
}

void set_material(Shape *shape, MaterialPhong material) {
  shape->material = material;
}

void set_transform(Shape *shape, Mat4 transformation) {
  shape->transformation = transformation;
}

Vector normal_at(Shape s, Point world_point) {
  assert(is_point(world_point));
  Vector result;
  switch (s.shape_type) {
  case SHAPE_TYPE_SPHERE: {
    Point object_point = m4_tmul(m4_inverse(s.transformation), world_point);
    Vector object_normal = tuple_sub(object_point, point(0, 0, 0));
    Vector world_normal =
        m4_tmul(m4_transpose(m4_inverse(s.transformation)), object_normal);
    world_normal.w = 0;
    result = tuple_normalize(world_normal);
    break;
  }
  case SHAPE_TYPE_PLANE:
    result = vector(0, 1, 0);
    break;
  }
  return result;
}
