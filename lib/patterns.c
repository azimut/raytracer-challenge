#include "./patterns.h"
#include <assert.h>
#include <math.h>

Pattern pattern_none(void) {
  Pattern p = {0};
  p.ptype = PATTERN_TYPE_NONE;
  p.transformation = m4_identity();
  return p;
}

Pattern pattern_stripes(Color a, Color b) {
  Pattern p = pattern_none();
  p.a = a, p.b = b, p.ptype = PATTERN_TYPE_STRIPES;
  return p;
}

Color pattern_at(Pattern ps, Point p) {
  assert(is_point(p));
  Color color;
  switch (ps.ptype) {
  case PATTERN_TYPE_STRIPES: {
    color = fmod(floor(p.x), 2) ? ps.b : ps.a;
    break;
  }
  default:
    color = RED;
    break;
  }
  return color;
}

void pattern_set_transformation(Pattern *ps, Mat4 m4) {
  ps->transformation = m4;
}
