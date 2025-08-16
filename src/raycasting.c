#include "./raycasting.h"

Ray ray(Point origin, Tuple direction) { return (Ray){origin, direction}; }

Point position(Ray r, float t) {
  return tuple_add(r.origin, tuple_smul(r.direction, t));
}
