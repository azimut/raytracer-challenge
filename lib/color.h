#ifndef COLOR_H
#define COLOR_H

#include <stdbool.h>

typedef struct Color {
  float red, green, blue;
} Color;

#define BLACK ((Color){0, 0, 0})
#define WHITE ((Color){1, 1, 1})
#define RED ((Color){1, 0, 0})
#define GREEN ((Color){0, 1, 0})
#define BLUE ((Color){0, 0, 1})

Color color(float, float, float);
Color color_add(Color, Color);
Color color_sub(Color, Color);
Color color_mul(Color, Color);
Color color_smul(Color, float);
Color color_sdiv(Color, float);
bool color_equal(Color, Color);
void color_print(Color);

#endif /* COLOR_H */
