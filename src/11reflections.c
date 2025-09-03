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

int main(int argc, char *argv[]) {
  (void)argc;
  World w = {0};

  PointLight p = {0};
  p = pointlight(point(-10, 7, 5), color(.63, 0.39450981, 0.887843144));
  p.attenuation_idx = 11;
  world_enlight(&w, p);
  p = pointlight(point(-3, 1, -3), color(0.2191, 0.19450981, 0.027843144));
  p.attenuation_idx = 8;
  world_enlight(&w, p);

  Shape floor = plane();
  floor.material = material();
  floor.material.pattern = pattern_checkers(WHITE, BLACK);
  /* floor.material.pattern.transformation = scaling(0.25, 1, 1); */
  /* floor.material.color = color(.4, 0.9, 0.9); */
  floor.material.specular = 0;
  floor.material.ambient = AMBIENT / 2;
  floor.material.reflective = 0.1;
  world_enter(&w, floor);

  /* double radius = 50; */
  /* double rot = 0; */
  /* for (size_t i = 0; i < 6; i += 1) { */
  /*   Shape hexa = plane(); */
  /*   hexa.transformation = */
  /*       m4_mul(translation(radius * cos(M_PI / 6.0 + (i * M_PI) / 3.0), 0, */
  /*                          radius * sin(M_PI / 6.0 + (i * M_PI) / 3.0)), */
  /*              m4_mul(rotation_y(radians(rot)), rotation_x(M_PI / 2))); */
  /*   rot -= 60; */
  /*   hexa.material.color = */
  /*       i % 2 ? (Color){0.25, 0.25, 0.25} : (Color){0.5, 0.5, 0.5}; */
  /*   hexa.material.pattern.transformation = */
  /*       m4_mul(scaling(10, 1, 1), M4_IDENTITY); */
  /*   hexa.material.reflective = i % 2 ? 0.1 : 0; */
  /*   hexa.material.specular = 0; */
  /*   world_enter(&w, hexa); */
  /* } */

  Shape roof = plane();
  roof.transformation = translation(0, 30, 0);
  roof.material.pattern = pattern_stripes(WHITE, BLACK);
  roof.material.pattern.transformation =
      m4_mul(translation(0, 0, 0), scaling(3, 1, 3));
  roof.material.color = color(136.0 / 255, 198.0 / 255, 252.0 / 255);
  roof.material.specular = 0;
  world_enter(&w, roof);

  Shape middle = sphere();
  middle.transformation = translation(-0.5, 1, 0.5);
  middle.material = material();
  middle.material.color = color(0.1, 1, 0.5);
  /* middle.material.pattern = pattern_stripes(RED, BLUE); */
  /* middle.material.pattern.transformation = */
  /*     m4_mul(rotation_z(M_PI / 3), scaling(0.11, 1, 1)); */
  middle.material.reflective = 0.9;
  middle.material.transparency = 1;
  middle.material.refractive_index = DEFAULT_REFRACTIVE_DIAMOND;
  middle.material.ambient = AMBIENT / 2;
  middle.material.diffuse = 0.1;
  middle.material.specular = 1;
  middle.material.shininess = 300;
  world_enter(&w, middle);

  Shape right = sphere();
  right.transformation =
      m4_mul(translation(-0.25, 0.5, -1.25), scaling(0.5, 0.5, 0.5));
  right.material = material();
  right.material.pattern = pattern_stripes(WHITE, BLACK);
  right.material.pattern.transformation =
      m4_mul(rotation_z(radians(-45)), scaling(.15, 1, 1));
  right.material.color = color(0.5, 1, 0.1);
  right.material.diffuse = 0.7;
  right.material.specular = 0.01;
  right.material.ambient = AMBIENT;
  /* right.material.reflective = 0.1; */
  world_enter(&w, right);

  Shape left = sphere();
  left.transformation =
      m4_mul(translation(-1.5, 0.33, -0.75), scaling(0.33, 0.33, 0.33));
  left.material = material();
  left.material.color = color(.15, .25, .15);
  left.material.diffuse = 0.7;
  left.material.ambient = AMBIENT;
  left.material.specular = .3;
  left.material.reflective = .25;
  /* left.material.refractive_index = DEFAULT_REFRACTIVE_GLASS; */
  /* left.material.transparency = 1; */
  world_enter(&w, left);

  char *filename = basename(argv[0]);
  Camera cam = camera(DIMENSION, DIMENSION, M_PI / 3);
  cam.transform = view_transform(point(-2.7, 1.0, -3.11), point(0, 0.75, 0),
                                 vector(0, 1, 0));

  /* int frame = 0; */
  /* for (float i = 0; i < M_PI * 2; i += .1) { */
  /*   Point from = point(sin(i) * 10, 0.5, cos(i) * 10); */
  /*   cam.transform = view_transform(from, point(-1, 1.5, 0), vector(0, 1, 0));
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
