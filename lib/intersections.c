#include "./intersections.h"
#include "./util.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

Intersection intersection(double t, Shape object) {
  return (Intersection){t, object};
}

void intersections_free(Intersections *is) {
  if (!is->hits)
    return;
  free(is->hits);
  is->hits = NULL, is->count = 0, is->capacity = 0;
}

bool intersection_equal(const Intersection i1, const Intersection i2) {
  return (i1.object.id == i2.object.id) && (i1.t == i2.t);
}

static int intersection_compare(const void *a, const void *b) {
  const Intersection *ia = a;
  const Intersection *ib = b;
  return ia->t > ib->t;
}

void intersections_sort(Intersections *is) {
  if (!is->count)
    return;
  qsort(is->hits, is->count, sizeof(Intersection), &intersection_compare);
}

Intersections intersections_new(size_t capacity) {
  assert(capacity < 1000); // ensure sanity
  Intersections is = {
      .count = 0,
      .capacity = capacity,
      .hits = calloc(capacity, sizeof(Intersection)),
  };
  if (!is.hits) {
    perror("_new");
    exit(EXIT_FAILURE);
  }
  return is;
}

void intersections_insert(Intersections *is, const Intersection i) {
  if ((is->count + 1) > is->capacity) {
    fprintf(stderr, "IS-----> cap=%li count=%li\n", is->capacity, is->count);
    is->capacity += RENEWED_CAPACITY;
    is->hits =
        reallocarray(is->hits, is->capacity, sizeof(struct Intersection));
    if (!is->hits) {
      perror("_push");
      exit(EXIT_FAILURE);
    }
  }
  is->hits[is->count] = i;
  is->count = is->count + 1;
}

static int intersections_index(const Intersections is, const Intersection i) {
  for (size_t idx = 0; idx < is.count; ++idx)
    if (intersection_equal(is.hits[idx], i))
      return idx;
  return -1;
}

bool intersections_includes(const Intersections is, const Intersection i) {
  return (intersections_index(is, i) >= 0);
}

void intersections_remove(Intersections *is, const Intersection i) {
  int found_idx = intersections_index(*is, i);
  if (found_idx == -1)
    return;
  for (size_t next_idx = found_idx + 1; next_idx < is->count; ++next_idx) {
    is->hits[next_idx - 1] = is->hits[next_idx];
  }
  is->count--;
}

Intersections intersections_filter(const Intersections xs, const Shape csg) {
  Intersections result = intersections_new(xs.count);
  if (csg.shape_type != SHAPE_TYPE_CSG) {
    return result;
  }
  bool inl = false, inr = false;
  for (size_t i = 0; i < xs.count; ++i) {
    const Shape *left = csg.shape_data.csg.left;
    const Csg_Op operation = csg.shape_data.csg.operation;
    const bool lhit = shape_includes(*left, xs.hits[i].object);
    if (csg_intersection_allowed(operation, lhit, inl, inr)) {
      intersections_insert(&result, xs.hits[i]);
    }
    if (lhit)
      inl = !inl;
    else
      inr = !inr;
  }
  return result;
}

Intersections intersections_combine(const Intersections xs,
                                    const Intersections ys) {
  Intersections result = intersections_new(xs.count + ys.count + 1);
  for (size_t i = 0; i < xs.count; ++i)
    intersections_insert(&result, xs.hits[i]);
  for (size_t i = 0; i < ys.count; ++i)
    intersections_insert(&result, ys.hits[i]);
  return result;
}

void intersections_append(Intersections *dst, const Intersections src) {
  for (size_t i = 0; i < src.count; ++i) {
    intersections_insert(dst, src.hits[i]);
  }
}
