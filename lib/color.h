#ifndef COLOR_H
#define COLOR_H

#include <stdbool.h>

typedef struct Color {
  double red, green, blue;
} Color;

#define BLACK ((Color){0, 0, 0})
#define WHITE ((Color){1, 1, 1})
#define RED ((Color){1, 0, 0})
#define GREEN ((Color){0, 1, 0})
#define BLUE ((Color){0, 0, 1})

Color color(double, double, double);
Color color_add(const Color, const Color);
Color color_sub(const Color, const Color);
Color color_mul(const Color, const Color);
Color color_smul(const Color, double);
Color color_sdiv(const Color, double);
bool color_equal(const Color, const Color);
void color_print(const Color);

#endif /* COLOR_H */
