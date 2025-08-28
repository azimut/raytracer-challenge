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
  Vector object_normal;
  switch (s.shape_type) {
  case SHAPE_TYPE_SPHERE: {
    Point object_point = m4_tmul(m4_inverse(s.transformation), world_point);
    object_normal = tuple_sub(object_point, point(0, 0, 0));
    break;
  }
  case SHAPE_TYPE_PLANE:
    object_normal = vector(0, 1, 0);
    break;
  }
  Vector world_normal =
      m4_tmul(m4_transpose(m4_inverse(s.transformation)), object_normal);
  world_normal.w = 0;
  return tuple_normalize(world_normal);
}

Color pattern_at_object(Pattern ps, Shape shape, Point p) {
  assert(is_point(p));
  Point object_point = m4_tmul(m4_inverse(shape.transformation), p);
  Point pattern_point = m4_tmul(m4_inverse(ps.transformation), object_point);
  return pattern_at(ps, pattern_point);
}
