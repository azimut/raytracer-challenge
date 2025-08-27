#ifndef SHAPES_H
#define SHAPES_H

#include "./material.h"
#include "./matrix.h"

typedef struct Sphere {
  int id;
  Mat4 transformation;
  MaterialPhong material;
} Sphere;

Sphere sphere();
void set_material(Sphere *, MaterialPhong);
void set_transform(Sphere *, Mat4);
Vector normal_at(Sphere, Point);

#endif /* SHAPES_H */
