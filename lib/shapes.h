#ifndef SHAPES_H
#define SHAPES_H

#include "./material.h"
#include "./matrix.h"

typedef enum {
  SHAPE_TYPE_SPHERE = 0,
  SHAPE_TYPE_PLANE,
} ShapeType;

typedef struct Shape {
  int id;
  Mat4 transformation;
  MaterialPhong material;
  ShapeType shape_type;
  union {
    struct {
    } sphere;
    struct {
    } plane;
  } shape_data;
} Shape;

typedef struct Shapes {
  size_t count;
  size_t capacity;
  Shape *shapes;
} Shapes;

Shape plane(void);
Shape sphere(void);
Shape sphere_glass(void);
void set_material(Shape *, MaterialPhong);
void set_transform(Shape *, Mat4);
Vector normal_at(const Shape, const Point);
Color pattern_at_shape(const Pattern, const Shape, const Point);

Shapes shapes_new(size_t);
void shapes_append(Shapes *, const Shape);
void shapes_free(Shapes *);
bool shapes_includes(const Shapes, const Shape);
void shapes_remove(Shapes *, const Shape);

#endif /* SHAPES_H */
