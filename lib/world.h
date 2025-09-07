#ifndef WORLD_H
#define WORLD_H

#include "./raycasting.h"
#include "./shading.h"
#include <stdint.h>

typedef struct World {
  Light *lights;
  size_t lights_count;
  Shapes shapes;
} World;

Intersections world_intersect(const World, const Ray);
void world_enter(World *, Shape);
void world_enlight(World *, Light);
World world_default(void);
void world_free(World *);
Color shade_hit(const World, const Computations, uint8_t);
Color color_at(const World, const Ray, uint8_t);
Color reflected_color(const World, const Computations, uint8_t);
Color refracted_color(const World, const Computations, uint8_t);
bool is_shadowed(const World, const Point, const Point);

#endif /* WORLD_H */
