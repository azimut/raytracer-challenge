#ifndef MATERIAL_H
#define MATERIAL_H

#include "./color.h"
#include "./patterns.h"

typedef struct MaterialPhong {
  float ambient, diffuse, specular, shininess, reflective;
  Color color;
  Pattern pattern;
} MaterialPhong;

MaterialPhong material(void);

#endif /* MATERIAL_H */
