#ifndef RAYCASTING_H
#define RAYCASTING_H

#include "./intersections.h"
#include "./matrix.h"
#include "./shading.h"
#include "./shapes.h"
#include "./tuple.h"
#include <stdlib.h>

typedef struct Ray {
  Point origin;
  Vector direction;
} Ray;
typedef struct Computations {
  Shape object;
  Point point, over_point, under_point;
  Vector eye;
  Vector normal;
  Vector reflect;
  bool is_inside;
  double t;
  double n1, n2; // refraction
} Computations;

Ray ray(Point, Vector);
Point position(Ray, double);
Intersections intersect(Shape, Ray);
Intersection *hit(Intersections);
Ray transform(Ray, Mat4);
Computations prepare_computations(Intersection, Ray, Intersections);

#endif /* RAYCASTING_H */
