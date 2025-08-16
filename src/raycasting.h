#ifndef RAYCASTING_H
#define RAYCASTING_H
#include "./tuple.h"

typedef struct Ray {
  Point origin;
  Vector direction;
} Ray;

Ray ray(Point, Tuple);
Point position(Ray, float);

#endif /* RAYCASTING_H */
