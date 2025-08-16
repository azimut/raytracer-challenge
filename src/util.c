#include "./util.h"
#include <math.h>
#include <stdbool.h>

bool near(float x, float y) { return fabs(x - y) < EPSILON; }
