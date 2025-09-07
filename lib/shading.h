#ifndef SHADING_H
#define SHADING_H

#include "./color.h"
#include "./material.h"
#include "./shapes.h"
#include "./tuple.h"
#include <stdint.h>

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

typedef struct Attenuations {
  double linear, quadratic;
} Attenuations;

typedef struct PointLight {
  Point position;
  Color intensity;
  uint8_t attenuation_idx;
} PointLight;

// from ∞ to smaller
static const Attenuations default_attenuations[13] = {
    {0, 0},          {0.0014, 0.000007}, {0.007, 0.0002}, {0.014, 0.0007},
    {0.022, 0.0019}, {0.027, 0.0028},    {0.045, 0.0075}, {0.07, 0.017},
    {0.09, 0.032},   {0.14, 0.07},       {0.22, 0.20},    {0.35, 0.44},
    {0.7, 1.8},
};

Vector reflect(const Vector, const Vector);

PointLight pointlight(const Point, const Color);

Color lighting(const MaterialPhong, const Shape, const Point, const PointLight,
               const Vector, const Vector, bool);

#endif /* SHADING_H */
