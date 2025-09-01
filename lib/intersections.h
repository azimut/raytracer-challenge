#ifndef INTERSECTIONS_H
#define INTERSECTIONS_H

#include "./shapes.h"
#include <stdarg.h>

#define RENEWED_CAPACITY 10

typedef struct Intersection {
  double t;
  Shape object;
} Intersection;
typedef struct Intersections {
  size_t count;
  size_t capacity;
  Intersection *hits;
} Intersections;

Intersection intersection(double, Shape);
Intersections intersections(Shape, int, ...);
void intersections_free(Intersections *);
void intersections_sort(Intersections *);
bool intersection_equal(Intersection, Intersection);

Intersections intersections_new(size_t);
void intersections_append(Intersections *, const Intersection);
void intersections_remove(Intersections *, const Intersection);
bool intersections_includes(const Intersections, const Intersection);

#endif /* INTERSECTIONS_H */
