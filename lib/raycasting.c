#include "./raycasting.h"
#include "./shading.h"
#include "./tuple.h"
#include "./util.h"
#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdarg.h>
#include <stddef.h>

Ray ray(Point origin, Vector direction) {
  assert(is_point(origin) && is_vector(direction));
  return (Ray){origin, direction};
}

Point position(Ray r, double t) {
  return tuple_add(r.origin, tuple_smul(r.direction, t));
}

Intersections intersect(Shape shape, Ray ray) {
  Ray tRay = transform(ray, m4_inverse(shape.transformation));
  Intersections is = {.count = 0, .hits = NULL};
  switch (shape.shape_type) {
  case SHAPE_TYPE_SPHERE: {
    Point sphere_to_ray = tuple_sub(tRay.origin, point(0, 0, 0));
    double a = tuple_dot_product(tRay.direction, tRay.direction);
    double b = 2.0 * tuple_dot_product(tRay.direction, sphere_to_ray);
    double c = tuple_dot_product(sphere_to_ray, sphere_to_ray) - 1.0;
    double discriminant = b * b - 4.0 * a * c;
    if (discriminant >= 0) {
      double i1 = (-b - sqrt(discriminant)) / (2.0 * a);
      double i2 = (-b + sqrt(discriminant)) / (2.0 * a);
      is = intersections(shape, 2, i1, i2);
      break;
    }
    break;
  }
  case SHAPE_TYPE_PLANE: {
    if (fabs(tRay.direction.y) < EPSILON) {
      break;
    }
    double i = -tRay.origin.y / tRay.direction.y; // only for xz planes
    is = intersections(shape, 1, i);
    break;
  }
  }
  return is;
}

Intersection *hit(Intersections is) {
  size_t idx = 0;
  double current = DBL_MAX;
  for (size_t i = 0; i < is.count; ++i) {
    if (is.hits[i].t > 0 && is.hits[i].t < current) {
      current = is.hits[i].t;
      idx = i;
    }
  }
  return (current == DBL_MAX) ? NULL : &is.hits[idx];
}

Ray transform(Ray ray, Mat4 m4) {
  return (Ray){
      .origin = m4_tmul(m4, ray.origin),
      .direction = m4_tmul(m4, ray.direction),
  };
}

Computations prepare_computations(Intersection ii, Ray r, Intersections is) {
  Computations comp = {
      .eye = tuple_neg(r.direction),
      .point = position(r, ii.t),
      .normal = normal_at(ii.object, position(r, ii.t)),
      .object = ii.object,
      .t = ii.t,
  };
  if (tuple_dot_product(comp.normal, comp.eye) < 0) {
    comp.is_inside = true;
    comp.normal = tuple_neg(comp.normal);
  } else {
    comp.is_inside = false;
  }
  comp.reflect = reflect(r.direction, comp.normal);
  comp.over_point = tuple_add(comp.point, tuple_smul(comp.normal, EPSILON));
  return comp;
}
