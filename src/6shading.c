#include "../lib/canvas.h"
#include "../lib/color.h"
#include "../lib/material.h"
#include "../lib/raycasting.h"
#include "../lib/shading.h"
#include "../lib/transformation.h"
#include "../lib/tuple.h"

#ifndef DIMENSION
#define DIMENSION 1024
#endif

int main(void) {
  PointLight light = pointlight(point(-10, 10, -10), color(1, 1, 1));
  Point ray_origin = POINT(0, 0, -5);
  float wall_z = 10;
  float wall_size = 7;
  Canvas c = canvas(DIMENSION, DIMENSION);
  float pixel_size = wall_size / DIMENSION; // 0.02333
  float half = wall_size / 2;               // 3.5
  Shape s = sphere();
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
      Point pos = POINT(world_x, world_y, wall_z);
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
                   lighting(s.material, s, obj_pos, light, eye, normal, false));
      }
      intersections_free(&is);
    }
  }
  canvas_save(c, "media/6shading.ppm");
  canvas_free(&c);
  return 0;
}
