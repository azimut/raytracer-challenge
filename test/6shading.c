#include "../src/canvas.h"
#include "../src/color.h"
#include "../src/material.h"
#include "../src/raycasting.h"
#include "../src/shading.h"
#include "../src/transformation.h"
#include "../src/tuple.h"

int main(void) {
  PointLight light = pointlight(point(-10, 10, -10), color(1, 1, 1));
  Point ray_origin = point(0, 0, -5);
  float wall_z = 10;
  float wall_size = 7;
  size_t canvas_pixels = 300;
  Canvas c = canvas(canvas_pixels, canvas_pixels);
  float pixel_size = wall_size / canvas_pixels; // 0.02333
  float half = wall_size / 2;                   // 3.5
  Sphere s = sphere();
  MaterialPhong m = material();
  m.color = color(1, 0.2, 1);
  set_material(&s, m);
  /* set_transform(&s, scaling(1, 0.5, 1)); */
  /* set_transform(&s, m4_mul(s.transformation, shearing(1, 2, 1, 1, -2, 1)));
   */
  Color black = (Color){0, 0, 0};
  Color red = (Color){1, 0, 0};
  for (size_t row = 0; row < c.width; row++) {
    float world_y = half - pixel_size * row;
    for (size_t col = 0; col < c.height; col++) {
      float world_x = half - pixel_size * col;
      Point pos = point(world_x, world_y, wall_z);
      // update ray direction
      Ray rr = ray(ray_origin, tuple_normalize(tuple_sub(pos, ray_origin)));
      // intersect with sphere
      Intersections is = intersect(s, rr);
      Intersection *i = hit(is);
      // if intersect canvas red
      if (i) {
        Point obj_pos = position(rr, i->t);
        Vector normal = normal_at(i->object, obj_pos);
        Vector eye = tuple_neg(rr.direction);
        canvas_set(&c, row, col,
                   lighting(s.material, obj_pos, light, eye, normal));
      }
      free_intersections(is);
    }
  }
  canvas_print(c);
  canvas_free(&c);
  return 0;
}
