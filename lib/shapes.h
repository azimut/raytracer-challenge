#ifndef SHAPES_H
#define SHAPES_H

#include "./material.h"
#include "./matrix.h"

typedef enum {
  SHAPE_TYPE_SPHERE = 0,
  SHAPE_TYPE_PLANE,
  SHAPE_TYPE_CUBE,
  SHAPE_TYPE_GROUP,
  SHAPE_TYPE_CSG,
  SHAPE_TYPE_TRIANGLE,
  SHAPE_TYPE_CYLINDER,
} ShapeType;

typedef enum {
  CSG_OP_UNION = 0,
  CSG_OP_INTERSECTION,
  CSG_OP_DIFFERENCE,
} Csg_Op;

typedef struct Shapes Shapes;

typedef struct Shape {
  int id;
  Mat4 transformation;
  MaterialPhong material;
  ShapeType shape_type;
  bool shadowless;
  struct Shape *parent;
  union {
    struct {
      bool closed;
      double minimum, maximum;
    } cylinder;
    struct Group {
      Shapes *childs;
    } group;
    struct {
      Csg_Op operation;
      struct Shape *left, *right;
    } csg;
    struct {
      Point p1, p2, p3;
      Vector e1, e2, normal;
    } triangle;
  } shape_data;
} Shape;

typedef struct Shapes {
  size_t count;
  size_t capacity;
  Shape *shapes;
} Shapes;

Shape triangle(const Point, const Point, const Point);
Shape cube(void);
Shape plane(void);
Shape sphere(void);
Shape sphere_glass(void);
Shape cylinder(void);
Shape group(void);
void group_add(Shape *, Shape *);
bool group_includes(const Shape, const Shape);
void group_free(Shape *);
Shape *csg(Csg_Op, Shape *, Shape *);
void csg_free(Shape *);

void set_material(Shape *, MaterialPhong);
void set_transform(Shape *, Mat4);
Vector normal_at(const Shape, const Point);
Color pattern_at_shape(const Pattern, const Shape, const Point);

Shapes shapes_new(size_t);
void shapes_append(Shapes *, const Shape);
void shapes_free(Shapes *);
bool shapes_includes(const Shapes, const Shape);
void shapes_remove(Shapes *, const Shape);

bool csg_intersection_allowed(Csg_Op, bool, bool, bool);
bool shape_includes(const Shape, const Shape);
bool shape_equal(const Shape, const Shape);

Point world_to_object(const Shape, Point);
Vector normal_to_world(const Shape, Vector);

#endif /* SHAPES_H */
