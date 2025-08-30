#include "./material.h"

MaterialPhong material(void) {
  return (MaterialPhong){
      .pattern = pattern_none(),
      .color = {1, 1, 1},
      .ambient = 0.1,
      .diffuse = 0.9,
      .specular = 0.9,
      .shininess = 200,
      .reflective = 0,
      .transparency = 0, // opaque by default = 0
      .refractive_index = DEFAULT_REFRACTIVE_VACUUM,
  };
}
