#ifndef SHADING_H
#define SHADING_H

#include "./color.h"
#include "./material.h"
#include "./tuple.h"

typedef struct PointLight {
  Point position;
  Color intensity;
} PointLight;

Vector reflect(Vector, Vector);

PointLight pointlight(Point, Color);

Color lighting(MaterialPhong, Point, PointLight, Vector, Vector, bool);

#endif /* SHADING_H */
