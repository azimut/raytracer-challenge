#include "./material.h"

MaterialPhong material(void) {
  return (MaterialPhong){
      .pattern = pattern_none(),
      .color = {1, 1, 1},
      .ambient = 0.1,
      .diffuse = 0.9,
      .specular = 0.9,
      .shininess = 200.0,
      .reflective = 0.0,
  };
}
