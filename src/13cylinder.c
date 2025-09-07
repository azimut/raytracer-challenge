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

#define AMBIENT 0.005

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
  p = pointlight(point(15, 7, -15), color(0.1, 0.9, 1.0));
  p.attenuation = LIGHT_SIZE_100;
  world_enlight(&w, p);
  p = pointlight(point(3, 12, -7), color(1.0, 0.1, 0.9));
  p.attenuation = LIGHT_SIZE_32;
  world_enlight(&w, p);

  Shape floor = plane();
  floor.material.pattern = pattern_checkers(WHITE, BLACK);
  floor.material.pattern.transformation = scaling(0.5, .5, .5);
  /* floor.material.color = color(.4, 0.9, 0.9); */
  floor.material.specular = 0;
  floor.material.ambient = AMBIENT / 2;
  floor.material.reflective = 0.1;
  world_enter(&w, floor);

  Shape s1 = cylinder();
  MaterialPhong m = material();
  m.ambient = AMBIENT;
  m.reflective = 0.1;
  s1.material = m;
  s1.shape_data.cylinder.maximum = 3;
  s1.shape_data.cylinder.minimum = 1;
  s1.shape_data.cylinder.closed = true;
  s1.transformation = rotation_x(radians(34));
  world_enter(&w, s1);

  char *filename = basename(argv[0]);
  Camera cam = camera(SIZEX, SIZEY, M_PI / 2);
  cam.transform =
      view_transform(point(-1.5, 0.5, 2), point(0.1, 1.5, 0), vector(0, 1, 0));

  int frame = 0;
  for (float i = 0; i < M_PI * 2; i += .1) {
    Point from = point(sin(i) * 4, 0.5, cos(i) * 4);
    cam.transform = view_transform(from, point(0, 2.5, 0), vector(0, 1, 0));
    Canvas canvas = render(cam, w);
    canvas_save_frame(canvas, filename, ++frame, from);
    canvas_free(&canvas);
  }

  char *buff = calloc(100, sizeof(char));
  strcat(strcat(strcat(buff, "media/"), filename), ".ppm");
  Canvas canvas = render(cam, w);
  canvas_save(canvas, buff);
  free(buff);
  world_free(&w);

  return 0;
}
