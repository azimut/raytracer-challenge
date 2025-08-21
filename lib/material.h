#ifndef MATERIAL_H
#define MATERIAL_H

#include "./color.h"

typedef struct MaterialPhong {
  float ambient, diffuse, specular, shininess;
  Color color;
} MaterialPhong;

MaterialPhong material(void);

#endif /* MATERIAL_H */
