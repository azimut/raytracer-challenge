#include "./raycasting.h"
#include "./shading.h"
#include "./tuple.h"
#include "./util.h"
#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdarg.h>
#include <stddef.h>

Ray ray(const Point origin, const Vector direction) {
#ifndef BLAZE
  assert(is_point(origin) && is_vector(direction));
#endif
  return (Ray){origin, direction};
}

Point position(Ray r, double t) {
  return tuple_add(r.origin, tuple_smul(r.direction, t));
}

typedef struct PlaneIntersect {
  double tmin, tmax;
} PlaneIntersect;

static PlaneIntersect check_axis(double origin, double direction) {
  const double tmin_numerator = -1.0 - origin;
  const double tmax_numerator = +1.0 - origin;
  PlaneIntersect result = {0};
  if (fabs(direction) >= EPSILON) {
    result.tmin = tmin_numerator / direction;
    result.tmax = tmax_numerator / direction;
  } else {
    result.tmin = tmin_numerator * INFINITY;
    result.tmax = tmax_numerator * INFINITY;
  }
  if (result.tmin > result.tmax) {
    const double tmp = result.tmax;
    result.tmax = result.tmin;
    result.tmin = tmp;
  }
  return result;
}

Intersections intersect(const Shape shape, const Ray ray) {
  const Ray tRay = transform(ray, m4_inverse(shape.transformation));
  Intersections is = intersections_new(5);
  switch (shape.shape_type) {
  case SHAPE_TYPE_SPHERE: {
    const Point sphere_to_ray = tuple_sub(tRay.origin, point(0, 0, 0));
    const double a = tuple_dot_product(tRay.direction, tRay.direction);
    const double b = 2.0 * tuple_dot_product(tRay.direction, sphere_to_ray);
    const double c = tuple_dot_product(sphere_to_ray, sphere_to_ray) - 1.0;
    const double discriminant = b * b - 4.0 * a * c;
    if (discriminant >= 0) {
      const double i1 = (-b - sqrt(discriminant)) / (2.0 * a);
      const double i2 = (-b + sqrt(discriminant)) / (2.0 * a);
      intersections_insert(&is, (Intersection){i1, shape});
      intersections_insert(&is, (Intersection){i2, shape});
      break;
    }
    break;
  }
  case SHAPE_TYPE_PLANE: {
    if (fabs(tRay.direction.y) < EPSILON) {
      break;
    }
    const double i = -tRay.origin.y / tRay.direction.y; // only for xz planes
    intersections_insert(&is, (Intersection){i, shape});
    break;
  }
  case SHAPE_TYPE_CUBE: {
    const PlaneIntersect xt = check_axis(tRay.origin.x, tRay.direction.x);
    const PlaneIntersect yt = check_axis(tRay.origin.y, tRay.direction.y);
    const PlaneIntersect zt = check_axis(tRay.origin.z, tRay.direction.z);
    const double tmin = fmax(fmax(xt.tmin, yt.tmin), zt.tmin);
    const double tmax = fmin(fmin(xt.tmax, yt.tmax), zt.tmax);
    if (tmin > tmax) { // miss
      break;
    }
    intersections_insert(&is, (Intersection){tmin, shape});
    intersections_insert(&is, (Intersection){tmax, shape});
    break;
  }
  case SHAPE_TYPE_GROUP: {
    if (!shape.shape_data.group.childs) {
      break;
    }
    Intersections result = intersections_new(5);
    for (size_t i = 0; i < shape.shape_data.group.childs->count; ++i) {
      Intersections tmp =
          intersect(shape.shape_data.group.childs->shapes[i], tRay);
      intersections_append(&result, tmp);
      intersections_free(&tmp);
    }
    intersections_sort(&result);
    intersections_append(&is, result);
    intersections_free(&result);
    break;
  }
  case SHAPE_TYPE_CSG: {
    Intersections lxs = intersect(*shape.shape_data.csg.left, ray);
    Intersections rxs = intersect(*shape.shape_data.csg.right, ray);
    Intersections combined = intersections_combine(lxs, rxs);
    intersections_sort(&combined);
    Intersections filtered = intersections_filter(combined, shape);
    intersections_append(&is, filtered);
    intersections_free(&lxs);
    intersections_free(&rxs);
    intersections_free(&combined);
    intersections_free(&filtered);
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

Ray transform(const Ray ray, const Mat4 m4) {
  return (Ray){
      .origin = m4_tmul(m4, ray.origin),
      .direction = m4_tmul(m4, ray.direction),
  };
}

static Computations compute_refractions(Intersections is,
                                        const Intersection hit) {
  Computations comp = {0};
  Shapes containers = shapes_new(10);
  for (size_t idx = 0; idx < is.count; ++idx) {
    const Intersection intersection = is.hits[idx];
    if (intersection_equal(hit, intersection)) {
      if (containers.count == 0) {
        comp.n1 = 1;
      } else {
        const Shape last = containers.shapes[containers.count - 1];
        comp.n1 = last.material.refractive_index;
      }
    }
    if (shapes_includes(containers, intersection.object)) {
      shapes_remove(&containers, intersection.object);
    } else {
      shapes_append(&containers, intersection.object);
    }
    if (intersection_equal(hit, intersection)) {
      if (containers.count == 0) {
        comp.n2 = 1;
      } else {
        const Shape last = containers.shapes[containers.count - 1];
        comp.n2 = last.material.refractive_index;
      }
      break;
    }
  }
  shapes_free(&containers);
  return comp;
}

double schlick(const Computations comp) {
  double cos = comp.cos_i;
  if (comp.n1 > comp.n2) {
    if (comp.sin2_t > 1) {
      return 1;
    }
    cos = comp.cos_t;
  }
  const double r0 = pow(((comp.n1 - comp.n2) / (comp.n1 + comp.n2)), 2);
  return r0 + (1 - r0) * pow((1 - cos), 5);
}

Computations prepare_computations(const Intersection ii, const Ray r,
                                  Intersections is) {
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
  comp.under_point = tuple_sub(comp.point, tuple_smul(comp.normal, EPSILON));
  const Computations temp_comp = compute_refractions(is, ii);
  comp.n1 = temp_comp.n1;
  comp.n2 = temp_comp.n2;
  // Snell's Law
  comp.n_ratio = comp.n1 / comp.n2;
  comp.cos_i = tuple_dot_product(comp.eye, comp.normal);
  comp.sin2_t = pow(comp.n_ratio, 2.0) * (1.0 - pow(comp.cos_i, 2.0));
  comp.cos_t = sqrt(1.0 - comp.sin2_t);
  return comp;
}
