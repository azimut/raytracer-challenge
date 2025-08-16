#include "./raycasting.h"
#include "./tuple.h"
#include <float.h>
#include <math.h>

int global_id = 0;

Ray ray(Point origin, Tuple direction) { return (Ray){origin, direction}; }

Point position(Ray r, float t) {
  return tuple_add(r.origin, tuple_smul(r.direction, t));
}

Sphere sphere() {
  global_id++;
  int id = global_id;
  return (Sphere){id};
}

SphereIntersection intersect(Sphere sphere, Ray ray) {
  SphereIntersection si;
  si.did_hit = false;
  si.hits[0] = si.hits[1] = FLT_MAX;
  Point sphere_to_ray = tuple_sub(ray.origin, point(0, 0, 0));
  float a = tuple_dot_product(ray.direction, ray.direction);
  float b = 2 * tuple_dot_product(ray.direction, sphere_to_ray);
  float c = tuple_dot_product(sphere_to_ray, sphere_to_ray) - 1;
  float discriminant = b * b - 4 * a * c;
  if (discriminant >= 0) {
    si.did_hit = true;
    si.hits[0] = (-b - sqrtf(discriminant)) / (2 * a);
    si.hits[1] = (-b + sqrtf(discriminant)) / (2 * a);
  }
  return si;
}
