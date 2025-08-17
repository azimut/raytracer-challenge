#ifndef RAYCASTING_H
#define RAYCASTING_H

#include "./matrix.h"
#include "./tuple.h"
#include <stdlib.h>

typedef struct Ray {
  Point origin;
  Vector direction;
} Ray;
typedef struct Sphere {
  int id;
  Mat4 transformation;
} Sphere;
typedef struct Intersection {
  float t;
  Sphere object;
} Intersection;
typedef struct Intersections {
  size_t count;
  Intersection *hits;
} Intersections;

Ray ray(Point, Tuple);
Point position(Ray, float);
Sphere sphere();
Intersection intersection(float, Sphere);
Intersections intersections(Sphere, int, ...);
Intersections intersect(Sphere, Ray);
void free_intersections(Intersections);
Intersection *hit(Intersections);
bool intersection_equal(Intersection, Intersection);
Ray transform(Ray, Mat4);
void set_transform(Sphere *, Mat4);

#endif /* RAYCASTING_H */
