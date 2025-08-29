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

Shape plane(void);
Shape sphere(void);
void set_material(Shape *, MaterialPhong);
void set_transform(Shape *, Mat4);
Vector normal_at(Shape, Point);
Color pattern_at_shape(Pattern, Shape, Point);

#endif /* SHAPES_H */
