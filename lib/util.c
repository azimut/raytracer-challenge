#include "./util.h"
#include <math.h>
#include <stdbool.h>

double clamp(double value, double min, double max) {
  return fmin(fmax(value, min), max);
}

bool near(const double x, const double y) { return fabs(x - y) < EPSILON; }
