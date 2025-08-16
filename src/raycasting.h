#ifndef RAYCASTING_H
#define RAYCASTING_H
#include "./tuple.h"

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

typedef struct SphereIntersection {
  bool did_hit;
  float hits[2];
} SphereIntersection;

SphereIntersection intersect(Sphere, Ray);

#endif /* RAYCASTING_H */
