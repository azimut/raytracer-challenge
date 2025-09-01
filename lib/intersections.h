#ifndef INTERSECTIONS_H
#define INTERSECTIONS_H

#include "./shapes.h"
#include <stdarg.h>

typedef struct Intersection {
  double t;
  Shape object;
} Intersection;
typedef struct Intersections {
  size_t count;
  Intersection *hits;
} Intersections;

Intersection intersection(double, Shape);
Intersections intersections(Shape, int, ...);
void free_intersections(Intersections *);
void intersections_sort(Intersections *);
bool intersection_equal(Intersection, Intersection);

#endif /* INTERSECTIONS_H */
