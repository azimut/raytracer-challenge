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
  p = pointlight(POINT(15, 7, -15), COLOR(0.1, 0.9, 1.0));
  p.attenuation = LIGHT_SIZE_200;
  world_enlight(&w, p);
  p = pointlight(POINT(3, 12, -7), COLOR(1.0, 0.1, 0.9));
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

  Shape roof = plane();
  roof.transformation = translation(0, 30, 0);
  roof.material.pattern = pattern_stripes(WHITE, BLACK);
  roof.material.pattern.transformation =
      m4_mul(translation(0, 0, 0), scaling(3, 1, 3));
  roof.material.color = COLOR(136.0 / 255, 198.0 / 255, 252.0 / 255);
  roof.material.specular = 0;
  roof.material.ambient = AMBIENT / 2;
  world_enter(&w, roof);

  Shape middle = sphere();
  middle.transformation = translation(-0.5, 1, -0.5);
  middle.material.color = COLOR(0.1, 1, 0.5);
  middle.material.color = BLACK;

  /* middle.material.pattern = pattern_stripes(RED, BLUE); */
  /* middle.material.pattern.transformation = */
  /*     m4_mul(rotation_z(M_PI / 3), scaling(0.11, 1, 1)); */
  middle.material.reflective = 0.4;
  middle.material.transparency = 1;
  middle.material.refractive_index = DEFAULT_REFRACTIVE_GLASS;
  middle.material.ambient = AMBIENT / 2;
  middle.material.diffuse = 0.05;
  middle.material.specular = 1;
  middle.material.shininess = 300;
  world_enter(&w, middle);

  Shape right = cube();
  right.transformation =
      m4_mul(translation(1.25, 0.5, -0.5),
             m4_mul(rotation(45, -45, 0), scaling(.49, 10.49, .49)));
  /* right.material.pattern = pattern_rings(WHITE, BLACK); */
  /* right.material.pattern.transformation = */
  /*     m4_mul(rotation_z(radians(-45)), scaling(.15, .15, .15)); */
  right.material.color = COLOR(0.4, 0.4, 0.4);
  right.material.diffuse = 0.7;
  right.material.specular = 1;
  right.material.ambient = AMBIENT / 2;
  right.material.reflective = 0.1;
  world_enter(&w, right);

  Shape left = sphere();
  left.transformation =
      m4_mul(translation(-1.5, 0.33, -0.75), scaling(.33, .33, .33));
  left.material.color = COLOR(.15, .25, .15);
  left.material.diffuse = 0.7;
  left.material.ambient = AMBIENT;
  left.material.specular = .3;
  left.material.reflective = .25;
  /* left.material.refractive_index = DEFAULT_REFRACTIVE_GLASS; */
  /* left.material.transparency = 1; */
  world_enter(&w, left);

  char *filename = basename(argv[0]);
  Camera cam = camera(SIZEX, SIZEY, M_PI / 2.75);
  cam.transform =
      view_transform(POINT(.5, 0.5, -5), POINT(0.1, 1.5, 0), VECTOR(0, 1, 0));

  /* int frame = 0; */
  /* for (float i = 0; i < M_PI * 2; i += .1) { */
  /*   Point from = point(sin(i) * 5, 0.5, cos(i) * 5); */
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
  return 0;
}
