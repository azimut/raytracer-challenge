#ifndef PATTERNS_H
#define PATTERNS_H

#include "./color.h"
#include "./matrix.h"
#include "./tuple.h"

typedef enum {
  PATTERN_TYPE_NONE = 0,
  PATTERN_TYPE_STRIPES,
  PATTERN_TYPE_GRADIENT,
  PATTERN_TYPE_RINGS,
  PATTERN_TYPE_CHECKERS,
} Pattern_Type;

typedef struct Pattern {
  Color a, b;
  Mat4 transformation;
  Pattern_Type ptype;
} Pattern;

Pattern pattern_none(void);
Pattern pattern_stripes(Color, Color);
Pattern pattern_gradient(Color, Color);
Pattern pattern_rings(Color, Color);
Pattern pattern_checkers(Color, Color);
Color pattern_at(Pattern, Point);
void pattern_set_transformation(Pattern *, Mat4);

#endif /* PATTERNS_H */
