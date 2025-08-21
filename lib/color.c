#include "./color.h"
#include "./util.h"
#include <stdio.h>

Color color(float r, float g, float b) { return (Color){r, g, b}; }
Color color_add(Color a, Color b) {
  return color(a.red + b.red, a.green + b.green, a.blue + b.blue);
}
Color color_sub(Color a, Color b) {
  return color(a.red - b.red, a.green - b.green, a.blue - b.blue);
}
Color color_mul(Color a, Color b) {
  return color(a.red * b.red, a.green * b.green, a.blue * b.blue);
}
Color color_smul(Color a, float by) {
  return color(a.red * by, a.green * by, a.blue * by);
}
Color color_sdiv(Color a, float by) {
  return color(a.blue / by, a.green / by, a.blue / by);
}
bool color_equal(Color a, Color b) {
  return near(a.red, b.red) && near(a.green, b.green) && near(a.blue, b.blue);
}
void color_print(Color c) {
  fprintf(stderr, "(%f,%f,%f)\n", c.red, c.green, c.blue);
}
