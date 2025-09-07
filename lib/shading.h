#ifndef SHADING_H
#define SHADING_H

#include "./color.h"
#include "./material.h"
#include "./shapes.h"
#include "./tuple.h"
#include <stdint.h>

typedef struct Attenuation {
  double linear, quadratic;
} Attenuation;

typedef enum {
  LIGHT_TYPE_POINT = 0,
  LIGHT_TYPE_AREA,
} Light_Type;

typedef struct Light {
  Point position;
  Color intensity;
  Attenuation attenuation;
  Light_Type ltype;
  union {
    struct {
    } point;
    struct {
      Point corner;
      Vector uvec, vvec;
      uint8_t usteps, vsteps;
      uint8_t samples;
    } area;
  } light_data;
} Light;

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

#define LIGHT_SIZE_INFINITE ((Attenuation){0, 0})
#define LIGHT_SIZE_3250 ((Attenuation){0.0014, 0.000007})
#define LIGHT_SIZE_600 ((Attenuation){0.007, 0.0002})
#define LIGHT_SIZE_325 ((Attenuation){0.014, 0.0007})
#define LIGHT_SIZE_200 ((Attenuation){0.022, 0.0019})
#define LIGHT_SIZE_160 ((Attenuation){0.027, 0.0028})
#define LIGHT_SIZE_100 ((Attenuation){0.045, 0.0075})
#define LIGHT_SIZE_65 ((Attenuation){0.07, 0.017})
#define LIGHT_SIZE_50 ((Attenuation){0.09, 0.032})
#define LIGHT_SIZE_32 ((Attenuation){0.14, 0.07})
#define LIGHT_SIZE_20 ((Attenuation){0.22, 0.20})
#define LIGHT_SIZE_13 ((Attenuation){0.35, 0.44})
#define LIGHT_SIZE_7 ((Attenuation){0.7, 1.8})

Vector reflect(const Vector, const Vector);

Light pointlight(const Point, const Color);
Light arealight(const Point corner, const Vector v1, const uint8_t usteps,
                const Vector v2, const uint8_t vsteps, const Color);

Color lighting(const MaterialPhong, const Shape, const Point, const Light,
               const Vector, const Vector, double);

#endif /* SHADING_H */
