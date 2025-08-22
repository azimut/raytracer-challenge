#include "./util.h"
#include <math.h>
#include <stdbool.h>

float clamp(float value, float min, float max) {
  return fmin(fmax(value, min), max);
}

bool near(float x, float y) { return fabs(x - y) < EPSILON; }
