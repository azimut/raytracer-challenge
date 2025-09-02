#include "./intersections.h"
#include "./util.h"
#include <assert.h>
#include <stdio.h>

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

void intersections_sort(Intersections *is) {
  if (!is->count)
    return;
  for (size_t i = 0; i < is->count - 1; ++i) {
    for (size_t j = i + 1; j < is->count; ++j) {
      if (is->hits[i].t > is->hits[j].t) {
        const Intersection in = is->hits[i];
        is->hits[i] = is->hits[j];
        is->hits[j] = in;
      }
    }
  }
}

Intersections intersections_new(size_t capacity) {
  assert(capacity < 1000 && capacity > 0); // ensure sanity
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

void intersections_append(Intersections *is, const Intersection i) {
  if ((is->count + 2) > is->capacity) {
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
