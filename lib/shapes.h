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

#endif /* SHAPES_H */
