#include "../src/canvas.h"
#include "../src/raycasting.h"
#include "../src/transformation.h"
#include "../src/tuple.h"

int main(void) {
  Point ray_origin = point(0, 0, -5);
  float wall_z = 10;
  float wall_size = 7;
  size_t canvas_pixels = 300;
  Canvas c = canvas(canvas_pixels, canvas_pixels);
  float pixel_size = wall_size / canvas_pixels; // 0.02333
  float half = wall_size / 2;                   // 3.5
  Sphere s = sphere();
  set_transform(&s, scaling(1, 0.5, 1));
  Color black = (Color){0, 0, 0};
  Color red = (Color){1, 0, 0};
  for (size_t row = 0; row < c.width; row++) {
    float world_y = half - pixel_size * row;
    for (size_t col = 0; col < c.height; col++) {
      float world_x = half - pixel_size * col;
      Point position = point(world_x, world_y, wall_z);
      // update ray direction
      Ray rr =
          ray(ray_origin, tuple_normalize(tuple_sub(position, ray_origin)));
      // intersect with sphere
      Intersections is = intersect(s, rr);
      Intersection *i = hit(is);
      // if intersect canvas red
      canvas_set(&c, row, col, i ? red : black);
      free_intersections(is);
    }
  }
  canvas_print(c);
  canvas_free(&c);
  return 0;
}
