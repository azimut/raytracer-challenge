#include "./shapes.h"
#include <assert.h>
#include <stdio.h>

int global_id = 0;

Shape plane(void) {
  return (Shape){
      .id = ++global_id,
      .transformation = M4_IDENTITY,
      .material = material(),
      .shape_type = SHAPE_TYPE_PLANE,
  };
}

Shape sphere(void) {
  return (Shape){
      .id = ++global_id,
      .transformation = M4_IDENTITY,
      .material = material(),
      .shape_type = SHAPE_TYPE_SPHERE,
  };
}

Shape sphere_glass(void) {
  Shape s = sphere();
  s.material.transparency = 1;
  s.material.refractive_index = 1.5;
  return s;
}

void set_material(Shape *shape, MaterialPhong material) {
  shape->material = material;
}

void set_transform(Shape *shape, Mat4 transformation) {
  shape->transformation = transformation;
}

Vector normal_at(const Shape shape, const Point world_point) {
#ifndef BLAZE
  assert(is_point(world_point));
#endif
  Vector object_normal;
  switch (shape.shape_type) {
  case SHAPE_TYPE_SPHERE: {
    const Point object_point =
        m4_tmul(m4_inverse(shape.transformation), world_point);
    object_normal = tuple_sub(object_point, point(0, 0, 0));
    break;
  }
  case SHAPE_TYPE_PLANE:
    object_normal = vector(0, 1, 0);
    break;
  }
  Vector world_normal =
      m4_tmul(m4_transpose(m4_inverse(shape.transformation)), object_normal);
  world_normal.w = 0;
  return tuple_normalize(world_normal);
}

Color pattern_at_shape(const Pattern pattern, const Shape shape,
                       const Point p) {
#ifndef BLAZE
  assert(is_point(p));
#endif
  const Point object_point = m4_tmul(m4_inverse(shape.transformation), p);
  const Point pattern_point =
      m4_tmul(m4_inverse(pattern.transformation), object_point);
  return pattern_at(pattern, pattern_point);
}

Shapes shapes_new(size_t capacity) {
  assert(capacity > 0 && capacity < 100);
  return (Shapes){
      .capacity = capacity,
      .shapes = calloc(capacity, sizeof(Shape)),
  };
}
void shapes_append(Shapes *shapes, const Shape shape) {
  if ((shapes->count + 2) > shapes->capacity) {
    if (shapes->capacity > 0)
      fprintf(stderr, "SH-----> cap=%li count=%li\n", shapes->capacity,
              shapes->count);
    shapes->capacity += 10;
    shapes->shapes =
        reallocarray(shapes->shapes, shapes->capacity, sizeof(Shape));
  }
  shapes->shapes[shapes->count] = shape;
  shapes->count++;
}
void shapes_free(Shapes *shapes) {
  if (!shapes)
    return;
  free(shapes->shapes);
  shapes->shapes = NULL, shapes->count = 0, shapes->capacity = 0;
}

static bool shape_equal(const Shape a, const Shape b) { return (a.id == b.id); }

static int shapes_index(const Shapes shapes, const Shape s) {
  for (size_t idx = 0; idx < shapes.count; ++idx)
    if (shape_equal(shapes.shapes[idx], s))
      return idx;
  return -1;
}

bool shapes_includes(const Shapes shapes, const Shape s) {
  return (shapes_index(shapes, s) >= 0);
}

void shapes_remove(Shapes *shapes, const Shape s) {
  int found_idx = shapes_index(*shapes, s);
  if (found_idx == -1)
    return;
  for (size_t next_idx = found_idx + 1; next_idx < shapes->count; ++next_idx) {
    shapes->shapes[next_idx - 1] = shapes->shapes[next_idx];
  }
  shapes->count--;
}
