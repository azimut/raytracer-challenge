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
  float t;
  Sphere object;
} Intersection;
typedef struct Intersections {
  size_t count;
  Intersection *hits;
} Intersections;
typedef struct Computations {
  Sphere object;
  Point point, over_point;
  Vector eye;
  Vector normal;
  bool is_inside;
  float t;
} Computations;

Ray ray(Point, Tuple);
Point position(Ray, float);
void set_material(Sphere *, MaterialPhong);
Intersection intersection(float, Sphere);
Intersections intersections(Sphere, int, ...);
Intersections intersect(Sphere, Ray);
void free_intersections(Intersections);
void intersections_sort(Intersections *);
Intersection *hit(Intersections);
bool intersection_equal(Intersection, Intersection);
Ray transform(Ray, Mat4);
void set_transform(Sphere *, Mat4);
Computations prepare_computations(Intersection, Ray);

#endif /* RAYCASTING_H */
