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
  double n1, n2, sin2_t, cos_t, cos_i, n_ratio; // refraction
} Computations;

Ray ray(Point, Vector);
Point position(Ray, double);
Intersections intersect(const Shape, const Ray);
Intersection *hit(Intersections);
Ray transform(Ray, Mat4);
Computations prepare_computations(const Intersection, const Ray, Intersections);
double schlick(const Computations);

#endif /* RAYCASTING_H */
