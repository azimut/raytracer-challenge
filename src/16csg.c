#include "../lib/camera.h"
#include "../lib/canvas.h"
#include "../lib/color.h"
#include "../lib/material.h"
#include "../lib/raycasting.h"
#include "../lib/shading.h"
#include "../lib/transformation.h"
#include "../lib/tuple.h"
#include "../lib/world.h"
#include <libgen.h>
#include <math.h>
#include <string.h>

#define AMBIENT 0.05

#ifndef DIMENSION
#define DIMENSION 200
#endif

#if !defined(SIZEX) || !defined(SIZEY)
#define SIZEX DIMENSION
#define SIZEY DIMENSION
#endif

int main(int argc, char *argv[]) {
  (void)argc;
  World w = {0};

  Light p = {0};
  p = pointlight(POINT(15, 7, -15), HEX2COLOR("#d601ff"));
  p.attenuation = LIGHT_SIZE_32;
  world_enlight(&w, p);
  p = pointlight(POINT(3, 12, -7), HEX2COLOR("#01ff9f"));
  p.attenuation = LIGHT_SIZE_100;
  world_enlight(&w, p);

  /* p = arealight(point(15, 7, -15), vector(0, 0, 4), 1, vector(3, 4, 0), 1, */
  /*               LIGHT_COLORS_FLUORESCENT_STANDARD); */
  /* p.attenuation = LIGHT_SIZE_32; */
  /* world_enlight(&w, p); */
  /* p = arealight(point(3, 12, -7), vector(2, 0, 0), 4, vector(0, 2, 0), 4, */
  /*               HEX2COLOR("#01ff9f")); */
  /* p.attenuation = LIGHT_SIZE_100; */
  /* world_enlight(&w, p); */

  Shape floor = plane();
  floor.material.pattern = pattern_checkers(WHITE, BLACK);
  floor.material.pattern.transformation = scaling(0.5, .5, .5);
  /* floor.material.color = color(.4, 0.9, 0.9); */
  floor.material.specular = 0;
  floor.material.ambient = AMBIENT;
  /* floor.material.reflective = 0.01; */
  world_enter(&w, floor);

  Shape s1 = sphere();
  Shape s2 = cube();
  MaterialPhong m = material();
  m.ambient = AMBIENT;
  /* m.reflective = 0.01; */
  s1.material = m;
  s2.material = m;
  s1.transformation = scaling(1.3, 1.3, 1.3);
  Shape *c = csg(CSG_OP_INTERSECTION, &s2, &s1);
  Shape g = group();
  g.transformation = m4_mul(translation(0, 1, 0), rotation_z(radians(0)));
  group_add(&g, c);
  world_enter(&w, g);

  char *filename = basename(argv[0]);
  Camera cam = camera(SIZEX, SIZEY, M_PI / 3);
  cam.transform =
      view_transform(POINT(2.5, 1.1, 2), POINT(0, 1., 0), VECTOR(0, 1, 0));

  /* int frame = 0; */
  /* for (float i = 0; i < M_PI * 2; i += .1) { */
  /*   Point from = point(sin(i) * 3, 1.5, cos(i) * 3); */
  /*   cam.transform = view_transform(from, point(0, 1.5, 0), vector(0, 1, 0));
   */
  /*   Canvas canvas = render(cam, w); */
  /*   canvas_save_frame(canvas, filename, ++frame, from); */
  /*   canvas_free(&canvas); */
  /* } */

  char *buff = calloc(100, sizeof(char));
  strcat(strcat(strcat(buff, "media/"), filename), ".ppm");
  Canvas canvas = render(cam, w);
  canvas_save(canvas, buff);
  free(buff);
  world_free(&w);
  csg_free(c);

  return 0;
}
