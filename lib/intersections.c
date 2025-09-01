#include "./intersections.h"
#include <assert.h>

Intersection intersection(double t, Shape object) {
  return (Intersection){t, object};
}

Intersections intersections(Shape object, int count, ...) {
  assert(count > 0);
  va_list ap;
  Intersections result;
  result.hits = calloc(count, sizeof(struct Intersection));
  result.count = count;
  va_start(ap, count);
  for (int i = 0; i < count; ++i) {
    result.hits[i].t = va_arg(ap, double); // automatic promotion
    result.hits[i].object = object;
  }
  va_end(ap);
  return result;
}

void free_intersections(Intersections *is) {
  if (!is->hits)
    return;
  free(is->hits);
  is->hits = NULL;
}

bool intersection_equal(Intersection i1, Intersection i2) {
  return (i1.object.id == i2.object.id) && (i1.t == i2.t);
}

void intersections_sort(Intersections *is) {
  if (!is->count)
    return;
  for (size_t i = 0; i < is->count - 1; ++i) {
    for (size_t j = i + 1; j < is->count; ++j) {
      if (is->hits[i].t > is->hits[j].t) {
        Intersection in;
        in = is->hits[i];
        is->hits[i] = is->hits[j];
        is->hits[j] = in;
      }
    }
  }
}
