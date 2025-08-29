#ifndef WORLD_H
#define WORLD_H

#include "./raycasting.h"
#include "./shading.h"
#include <stdint.h>

typedef struct World {
  PointLight light;
  size_t count;
  Shape *objects;
} World;

Intersections world_intersect(World, Ray);
void world_enter(World *, Shape);
World world_default(void);
void world_free(World *);
Color shade_hit(World, Computations, uint8_t);
Color color_at(World, Ray, uint8_t);
Color reflected_color(World, Computations, uint8_t);
bool is_shadowed(World, Point);

#endif /* WORLD_H */
