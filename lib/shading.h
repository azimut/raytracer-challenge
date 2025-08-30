#ifndef SHADING_H
#define SHADING_H

#include "./color.h"
#include "./material.h"
#include "./shapes.h"
#include "./tuple.h"
#include <stdint.h>

typedef struct Attenuations {
  double linear, quadratic;
} Attenuations;

// from ∞ to smaller
static const Attenuations default_attenuations[13] = {
    {0, 0},          {0.0014, 0.000007}, {0.007, 0.0002}, {0.014, 0.0007},
    {0.022, 0.0019}, {0.027, 0.0028},    {0.045, 0.0075}, {0.07, 0.017},
    {0.09, 0.032},   {0.14, 0.07},       {0.22, 0.20},    {0.35, 0.44},
    {0.7, 1.8},
};

typedef struct PointLight {
  Point position;
  Color intensity;
  uint8_t attenuation_idx;
} PointLight;

Vector reflect(Vector, Vector);

PointLight pointlight(Point, Color);

Color lighting(MaterialPhong, Shape, Point, PointLight, Vector, Vector, bool);

#endif /* SHADING_H */
