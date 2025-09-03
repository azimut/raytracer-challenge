#ifndef WORLD_H
#define WORLD_H

#include "./raycasting.h"
#include "./shading.h"
#include <stdint.h>

typedef struct World {
  PointLight *lights;
  size_t lights_count;
  Shapes shapes;
} World;

// Source: http://planetpixelemporium.com/tutorialpages/light.html
#define LIGHT_COLORS_CANDLE ((Color){1.0, 0.5764706, 0.16078432})
#define LIGHT_COLORS_TUNGSTEN_40 ((Color){1.0, 0.7725491, 0.56078434})
#define LIGHT_COLORS_TUNGSTEN_100 ((Color){1.0, 0.83921576, 0.6666667})
#define LIGHT_COLORS_HALOGEN ((Color){1.0, 0.9450981, 0.87843144})
#define LIGHT_COLORS_CARBON_ARC ((Color){1.0, 0.9803922, 0.9568628})
#define LIGHT_COLORS_HIGH_NOON_SUN ((Color){1.0, 1.0, 0.9843138})
#define LIGHT_COLORS_DIRECT_SUNLIGHT ((Color){1.0, 1.0, 1.0})
#define LIGHT_COLORS_SKY_OVERCAST ((Color){0.7882353, 0.8862746, 1.0})
#define LIGHT_COLORS_SKY_CLEAR_BLUE ((Color){0.2509804, 0.6117647, 1.0})
#define LIGHT_COLORS_FLUORESCENT_WARM ((Color){1.0, 0.9568628, 0.8980393})
#define LIGHT_COLORS_FLUORESCENT_STANDARD ((Color){0.9568628, 1.0, 0.9803922})
#define LIGHT_COLORS_FLUORESCENT_COOL_WHITE ((Color){0.8313726, 0.9215687, 1.0})
#define LIGHT_COLORS_FLUORESCENT_FULL_SPECTRUM                                 \
  ((Color){1.0, 0.9568628, 0.9490197})
#define LIGHT_COLORS_FLUORESCENT_GROW_LIGHT                                    \
  ((Color){1.0, 0.93725497, 0.9686275})
#define LIGHT_COLORS_FLUORESCENT_BLACK_LIGHT ((Color){0.654902, 0.0, 1.0})
#define LIGHT_COLORS_VAPOR_MERCURY ((Color){0.8470589, 0.9686275, 1.0})
#define LIGHT_COLORS_VAPOR_SODIUM ((Color){1.0, 0.8196079, 0.69803923})
#define LIGHT_COLORS_METAL_HALIDE ((Color){0.9490197, 0.98823535, 1.0})
#define LIGHT_COLORS_HIGH_PRESSURE_SODIUM ((Color){1.0, 0.7176471, 0.29803923})

Intersections world_intersect(const World, const Ray);
void world_enter(World *, Shape);
void world_enlight(World *, PointLight);
World world_default(void);
void world_free(World *);
Color shade_hit(const World, const Computations, uint8_t);
Color color_at(const World, const Ray, uint8_t);
Color reflected_color(const World, const Computations, uint8_t);
Color refracted_color(const World, const Computations, uint8_t);
bool is_shadowed(const World, const Point, const Point);

#endif /* WORLD_H */
