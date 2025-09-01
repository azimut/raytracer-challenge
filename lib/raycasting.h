#ifndef RAYCASTING_H
#define RAYCASTING_H

#include "./matrix.h"
#include "./shading.h"
#include "./shapes.h"
#include "./tuple.h"
#include <stdlib.h>

typedef struct Ray {
  Point origin;
  Vector direction;
} Ray;
typedef struct Intersection {
  double t;
  Shape object;
} Intersection;
typedef struct Intersections {
  size_t count;
  Intersection *hits;
} Intersections;
typedef struct Computations {
  Shape object;
  Point point, over_point;
  Vector eye;
  Vector normal;
  Vector reflect;
  bool is_inside;
  double t;
  double n1, n2; // refraction
} Computations;

Ray ray(Point, Vector);
Point position(Ray, double);
Intersection intersection(double, Shape);
Intersections intersections(Shape, int, ...);
Intersections intersect(Shape, Ray);
void free_intersections(Intersections *);
void intersections_sort(Intersections *);
Intersection *hit(Intersections);
bool intersection_equal(Intersection, Intersection);
Ray transform(Ray, Mat4);
Computations prepare_computations(Intersection, Ray, Intersections);

#endif /* RAYCASTING_H */
