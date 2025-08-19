#ifndef SHADING_H
#define SHADING_H

#include "./color.h"
#include "./material.h"
#include "./shapes.h"
#include "./tuple.h"

typedef struct PointLight {
  Point position;
  Color intensity;
} PointLight;

Vector normal_at(Sphere, Point);
Vector reflect(Vector, Vector);

PointLight pointlight(Point, Color);

Color lighting(MaterialPhong, Point, PointLight, Vector, Vector);

#endif /* SHADING_H */
