#include "./patterns.h"
#include <assert.h>
#include <math.h>

Pattern pattern_none(void) {
  Pattern p = {0};
  p.ptype = PATTERN_TYPE_NONE;
  p.transformation = M4_IDENTITY;
  return p;
}

Pattern pattern_test(void) {
  Pattern p = pattern_none();
  p.ptype = PATTERN_TYPE_TEST;
  return p;
}

Pattern pattern_stripes(Color a, Color b) {
  Pattern p = pattern_none();
  p.a = a, p.b = b, p.ptype = PATTERN_TYPE_STRIPES;
  return p;
}

Pattern pattern_gradient(Color a, Color b) {
  Pattern p = pattern_none();
  p.a = a, p.b = b, p.ptype = PATTERN_TYPE_GRADIENT;
  return p;
}

Pattern pattern_rings(Color a, Color b) {
  Pattern p = pattern_none();
  p.a = a, p.b = b, p.ptype = PATTERN_TYPE_RINGS;
  return p;
}

Pattern pattern_checkers(Color a, Color b) {
  Pattern p = pattern_none();
  p.a = a, p.b = b, p.ptype = PATTERN_TYPE_CHECKERS;
  return p;
}

Color pattern_at(Pattern pattern, Point point) {
  assert(is_point(point));
  Color color;
  switch (pattern.ptype) {
  case PATTERN_TYPE_TEST: {
    color = (Color){point.x, point.y, point.z};
    break;
  }
  case PATTERN_TYPE_STRIPES: {
    color = fmod(floor(point.x), 2) ? pattern.b : pattern.a;
    break;
  }
  case PATTERN_TYPE_GRADIENT: {
    Color distance = color_sub(pattern.b, pattern.a);
    double fraction = point.x - floor(point.x);
    color = color_add(pattern.a, color_smul(distance, fraction));
    break;
  }
  case PATTERN_TYPE_RINGS: {
    color = fmod(floor(sqrt(pow(point.x, 2) + pow(point.z, 2))), 2) ? pattern.b
                                                                    : pattern.a;
    break;
  }
  case PATTERN_TYPE_CHECKERS: {
    color = fmod(floor(point.x) + floor(point.y) + floor(point.z), 2)
                ? pattern.b
                : pattern.a;
    break;
  }
  case PATTERN_TYPE_NONE:
    color = RED;
    break;
  }
  return color;
}

void pattern_set_transformation(Pattern *ps, Mat4 m4) {
  ps->transformation = m4;
}
