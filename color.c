#include "./color.h"

Color color(float r, float g, float b) {
  return (Color){ r, g, b};
}
Color color_add(Color a , Color b) {
  return color(a.red + b.red, a.green + b.green, a.blue + b.blue);
}
Color color_sub(Color a, Color b) {
  return color(a.red - b.red, a.green - b.green, a.blue - b.blue) ;
}
Color color_mul(Color a, Color b) {
  return color(a.red * b.red, a.green * b.green, a.blue * b.blue);
}
Color color_smul(Color a, float by) {
  return color(a.blue * by, a.green * by , a.blue * by);
}
