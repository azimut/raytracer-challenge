#ifndef WORLD_H
#define WORLD_H

#include "./raycasting.h"
#include "./shading.h"

typedef struct World {
  PointLight light;
  size_t count;
  Sphere *objects;
} World;

Intersections world_intersect(World, Ray);
void world_enter(World *, Sphere);
World world_default(void);
void world_free(World *);
Color shade_hit(World, Computations);
Color color_at(World, Ray);
bool is_shadowed(World, Point);

#endif /* WORLD_H */
