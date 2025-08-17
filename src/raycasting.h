#ifndef RAYCASTING_H
#define RAYCASTING_H

#include "./tuple.h"
#include <stdlib.h>

typedef struct Ray {
  Point origin;
  Vector direction;
} Ray;
typedef struct Sphere {
  int id;
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
Intersections intersect(Sphere, Ray);
void free_intersections(Intersections);

#endif /* RAYCASTING_H */
