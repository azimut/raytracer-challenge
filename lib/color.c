#include "./color.h"
#include "./util.h"
#include <stdio.h>

Color color(double r, double g, double b) { return (Color){r, g, b}; }
Color color_add(const Color a, const Color b) {
  return color(a.red + b.red, a.green + b.green, a.blue + b.blue);
}
Color color_sub(const Color a, const Color b) {
  return color(a.red - b.red, a.green - b.green, a.blue - b.blue);
}
Color color_mul(const Color a, const Color b) {
  return color(a.red * b.red, a.green * b.green, a.blue * b.blue);
}
Color color_smul(const Color a, double by) {
  return color(a.red * by, a.green * by, a.blue * by);
}
Color color_sdiv(const Color a, double by) {
  return color(a.blue / by, a.green / by, a.blue / by);
}
bool color_equal(const Color a, const Color b) {
  return near(a.red, b.red) && near(a.green, b.green) && near(a.blue, b.blue);
}
void color_print(const Color c) {
  fprintf(stderr, "(%f,%f,%f)\n", c.red, c.green, c.blue);
}
