#ifndef RAYCASTING_H
#define RAYCASTING_H
#include "./tuple.h"
#include <stdlib.h>

typedef struct Ray {
  Point origin;
  Vector direction;
} Ray;

Ray ray(Point, Tuple);
Point position(Ray, float);

typedef struct Sphere {
  int id;
} Sphere;
Sphere sphere();

typedef struct Intersection {
  float t;
  Sphere object;
} Intersection;
Intersection intersection(float, Sphere);

typedef struct Intersections {
  size_t count;
  Intersection *hits;
} Intersections;

Intersections intersect(Sphere, Ray);

void free_intersections(Intersections);

#endif /* RAYCASTING_H */
