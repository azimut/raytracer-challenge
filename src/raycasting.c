#include "./raycasting.h"
#include "./tuple.h"
#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdarg.h>
#include <stddef.h>

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

Intersection intersection(float t, Sphere object) {
  return (Intersection){t, object};
}

// NOTE: if called with literals, use "1.0" notation, NOT bare "1"
Intersections intersections(Sphere object, int count, ...) {
  assert(count > 0);
  va_list ap;
  Intersections result;
  result.hits = malloc(sizeof(struct Intersection) * count);
  result.count = count;
  va_start(ap, count);
  for (int i = 0; i < count; ++i) {
    result.hits[i].t = (float)va_arg(ap, double); // due automatic promotion
    result.hits[i].object = object;
  }
  va_end(ap);
  return result;
};

Intersections intersect(Sphere sphere, Ray ray) {
  Point sphere_to_ray = tuple_sub(ray.origin, point(0, 0, 0));
  float a = tuple_dot_product(ray.direction, ray.direction);
  float b = 2 * tuple_dot_product(ray.direction, sphere_to_ray);
  float c = tuple_dot_product(sphere_to_ray, sphere_to_ray) - 1;
  float discriminant = b * b - 4 * a * c;
  if (discriminant >= 0) {
    float i1 = (-b - sqrtf(discriminant)) / (2 * a);
    float i2 = (-b + sqrtf(discriminant)) / (2 * a);
    return intersections(sphere, 2, i1, i2);
  }
  return (Intersections){.count = 0, .hits = NULL};
}

void free_intersections(Intersections is) {
  if (is.count > 0)
    free(is.hits);
}

bool intersection_equal(Intersection i1, Intersection i2) {
  return (i1.object.id == i2.object.id) && (i1.t == i2.t);
}

Intersection *hit(Intersections is) {
  if (is.count == 0)
    return NULL;
  size_t idx;
  float current = FLT_MAX;
  for (size_t i = 0; i < is.count; ++i) {
    if (is.hits[i].t < current) {
      current = is.hits[i].t;
      idx = i;
    }
  }
  return &is.hits[idx];
}
