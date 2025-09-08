#include "./shapes.h"
#include "./util.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int global_id = 0;

Shape cube(void) {
  return (Shape){
      .id = ++global_id,
      .transformation = M4_IDENTITY,
      .material = material(),
      .shape_type = SHAPE_TYPE_CUBE,
  };
}

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

Shape cylinder(void) {
  return (Shape){
      .id = ++global_id,
      .transformation = M4_IDENTITY,
      .material = material(),
      .shape_type = SHAPE_TYPE_CYLINDER,
      .shape_data.cylinder.minimum = -INFINITY,
      .shape_data.cylinder.maximum = +INFINITY,
  };
}

Shape triangle(const Point a, const Point b, const Point c) {
  return (Shape){
      .id = ++global_id,
      .material = material(),
      .transformation = M4_IDENTITY,
      .shape_type = SHAPE_TYPE_TRIANGLE,
      .shape_data.triangle.p1 = a,
      .shape_data.triangle.p2 = b,
      .shape_data.triangle.p3 = c,
      .shape_data.triangle.e1 = tuple_sub(b, a),
      .shape_data.triangle.e2 = tuple_sub(c, a),
      .shape_data.triangle.normal = tuple_normalize(
          tuple_cross_product(tuple_sub(c, a), tuple_sub(b, a))),
  };
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
  Vector object_normal = {0};
  const Point object_point = world_to_object(shape, world_point);
  switch (shape.shape_type) {
  case SHAPE_TYPE_CSG:
  case SHAPE_TYPE_GROUP: {
    fprintf(stderr, "normal_at with invalid shape_type\n");
    exit(EXIT_FAILURE);
  }
  case SHAPE_TYPE_SPHERE: {
    object_normal = tuple_sub(object_point, POINT(0, 0, 0));
    break;
  }
  case SHAPE_TYPE_CYLINDER: {
    // square of the distance from Y axis
    const double distance = pow(object_point.x, 2) + pow(object_point.z, 2);
    const double max = shape.shape_data.cylinder.maximum;
    const double min = shape.shape_data.cylinder.minimum;
    if (distance < 1 && object_point.y >= max - EPSILON) {
      object_normal = VECTOR(0, 1, 0);
    } else if (distance < 1 && object_point.y <= min + EPSILON) {
      object_normal = VECTOR(0, -1, 0);
    } else {
      object_normal = VECTOR(object_point.x, 0, object_point.z);
    }
    break;
  }
  case SHAPE_TYPE_PLANE: {
    object_normal = VECTOR(0, 1, 0);
    break;
  }
  case SHAPE_TYPE_CUBE: {
    const double maxc = fmax(fmax(fabs(object_point.x), fabs(object_point.y)),
                             fabs(object_point.z));
    if (near(maxc, fabs(object_point.x)))
      object_normal = VECTOR(object_point.x, 0, 0);
    else if (near(maxc, fabs(object_point.y)))
      object_normal = VECTOR(0, object_point.y, 0);
    else
      object_normal = VECTOR(0, 0, object_point.z);
    break;
  }
  case SHAPE_TYPE_TRIANGLE:
    object_normal = shape.shape_data.triangle.normal;
    break;
  }
  return normal_to_world(shape, object_normal);
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
  if ((shapes->count + 1) > shapes->capacity) {
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

bool shape_equal(const Shape a, const Shape b) { return (a.id == b.id); }

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

Shape *csg(Csg_Op operation, Shape *left, Shape *right) {
  Shape *shape = malloc(sizeof(struct Shape));
  shape->id = ++global_id;
  shape->material = material();
  shape->transformation = M4_IDENTITY;
  shape->shape_type = SHAPE_TYPE_CSG;
  shape->shape_data.csg.operation = operation;
  shape->shape_data.csg.left = left;
  shape->shape_data.csg.right = right;
  left->parent = shape;
  right->parent = shape;
  return shape;
}

void csg_free(Shape *csg) {
  if (!csg || csg->shape_type != SHAPE_TYPE_CSG)
    return;
  free(csg);
  csg = NULL;
}

// lhit = true if .left shape was hit
// inl  = true if hit happened inside .left
// inr  = true if hit happened inside .right
bool csg_intersection_allowed(Csg_Op operation, bool lhit, bool inl, bool inr) {
  bool result = false;
  switch (operation) {
  case CSG_OP_UNION:
    result = (lhit && !inr) || (!lhit && !inl);
    break;
  case CSG_OP_INTERSECTION:
    result = (lhit && inr) || (!lhit && inl);
    break;
  case CSG_OP_DIFFERENCE:
    result = (lhit && !inr) || (!lhit && inl);
    break;
  }
  return result;
};

bool shape_includes(const Shape shape, const Shape needle) {
  bool found = false;
  switch (shape.shape_type) {
  case SHAPE_TYPE_TRIANGLE:
    fprintf(stderr, "ERROR: shape_includes() unimplemented for TRIANGLES.\n");
    exit(EXIT_FAILURE);
  case SHAPE_TYPE_GROUP: {
    for (size_t i = 0; i < shape.shape_data.group.childs->count; ++i) {
      found = shape_includes(shape.shape_data.group.childs->shapes[i], needle);
      if (found)
        break;
    }
    break;
  }
  case SHAPE_TYPE_CSG:
    found = shape_includes(*shape.shape_data.csg.left, needle) ||
            shape_includes(*shape.shape_data.csg.right, needle);
    break;
  case SHAPE_TYPE_CUBE:
  case SHAPE_TYPE_PLANE:
  case SHAPE_TYPE_SPHERE:
  case SHAPE_TYPE_CYLINDER:
    found = shape_equal(shape, needle);
    break;
  }
  return found;
};

Shape group(void) {
  return (Shape){
      .id = ++global_id,
      .transformation = M4_IDENTITY,
      .material = material(),
      .shape_type = SHAPE_TYPE_GROUP,
      .parent = NULL,
      .shape_data.group.childs = NULL,
  };
}

void group_free(Shape *g) {
  assert(g->shape_type == SHAPE_TYPE_GROUP);
  if (g->shape_data.group.childs != NULL) {
    free(g->shape_data.group.childs->shapes);
    free(g->shape_data.group.childs);
    g->shape_data.group.childs = NULL;
  }
}

void group_add(Shape *g, Shape *child) {
  assert(g->shape_type == SHAPE_TYPE_GROUP);
  if (!(g->shape_data.group.childs)) {
    g->shape_data.group.childs = malloc(sizeof(Shapes));
    *g->shape_data.group.childs = shapes_new(10);
  }
  child->parent = g;
  shapes_append(g->shape_data.group.childs, *child);
}

// NOTE: no recursive
bool group_includes(const Shape g, const Shape child) {
  assert(g.shape_type == SHAPE_TYPE_GROUP);
  return shapes_includes(*g.shape_data.group.childs, child);
}

Point world_to_object(const Shape shape, Point p) {
  if (shape.parent) {
    p = world_to_object(*shape.parent, p);
  }
  return m4_tmul(m4_inverse(shape.transformation), p);
}

Vector normal_to_world(const Shape shape, Vector normal) {
  normal = m4_tmul(m4_transpose(m4_inverse(shape.transformation)), normal);
  normal.w = 0;
  normal = tuple_normalize(normal);
  if (shape.parent) {
    normal = normal_to_world(*shape.parent, normal);
  }
  return normal;
}
