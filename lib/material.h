#ifndef MATERIAL_H
#define MATERIAL_H

#include "./color.h"
#include "./patterns.h"

#define DEFAULT_REFRACTIVE_VACUUM 1
#define DEFAULT_REFRACTIVE_AIR 1.00029
#define DEFAULT_REFRACTIVE_WATER 1.333
#define DEFAULT_REFRACTIVE_GLASS 1.52
#define DEFAULT_REFRACTIVE_DIAMOND 2.417

typedef struct MaterialPhong {
  double ambient, diffuse, specular, shininess;
  double reflective;
  double transparency, refractive_index;
  Color color;
  Pattern pattern;
} MaterialPhong;

MaterialPhong material(void);

#endif /* MATERIAL_H */
