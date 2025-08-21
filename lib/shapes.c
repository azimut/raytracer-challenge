#include "./shapes.h"

int global_id = 0;

Sphere sphere() {
  return (Sphere){
      .id = ++global_id,
      .transformation = m4_identity(),
      .material = material(),
  };
}
