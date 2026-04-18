#include "../lib/camera.h"
#include "../lib/canvas.h"
#include "../lib/color.h"
#include "../lib/material.h"
#include "../lib/obj.h"
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

#ifndef LSAMPLES
#define LSAMPLES 4
#endif

int main(int argc, char *argv[]) {
  (void)argc;
  World w = {0};

  Light p = {0};
  /* p = pointlight(POINT(15, 7, -15), HEX2COLOR("#d601ff")); */
  /* p.attenuation = LIGHT_SIZE_32; */
  /* world_enlight(&w, p); */
  /* p = pointlight(POINT(3, 12, -7), HEX2COLOR("#01ff9f")); */
  /* p.attenuation = LIGHT_SIZE_100; */
  /* world_enlight(&w, p); */

  p = arealight(POINT(15, 3, -15), VECTOR(0, 0, 4), LSAMPLES, VECTOR(3, 4, 0),
                LSAMPLES, HEX2COLOR("#d601ff"));
  p.attenuation = LIGHT_SIZE_50;
  world_enlight(&w, p);
  p = arealight(POINT(-13, 5, 17), VECTOR(2, 0, 0), LSAMPLES, VECTOR(0, 2, 0),
                LSAMPLES, HEX2COLOR("#01ff9f"));
  p.attenuation = LIGHT_SIZE_100;
  world_enlight(&w, p);

  Shape floor = plane();
  floor.material.pattern = pattern_checkers(WHITE, BLACK);
  floor.material.pattern.transformation = scaling(0.25, .25, .25);
  /* floor.material.color = color(.4, 0.9, 0.9); */
  floor.material.specular = 0;
  floor.material.ambient = AMBIENT;
  /* floor.material.reflective = 0.01; */
  world_enter(&w, floor);

  Parser figure =
      obj_parse_file("/home/sendai/Downloads/Nicolas-Llavero-SVG.obj");
  /* Parser figure = obj_parse_file("/home/sendai/Downloads/gamedev/models/" */
  /*                                "collection1/objs/dodecahedron.obj"); */
  figure.default_group->material.specular = 1;
  figure.default_group->material.shininess = 200;
  figure.default_group->material.ambient = AMBIENT;
  figure.default_group->material.reflective = 0.8;
  figure.default_group->material.refractive_index = DEFAULT_REFRACTIVE_DIAMOND;
  figure.default_group->material.transparency = 1;
  figure.default_group->material.diffuse = 0.1;
  figure.default_group->transformation =
      m4_mul(translation(0, 1, 0),
             m4_mul(rotation(0, 45, 0), scaling(0.05, 0.05, 0.05)));
  world_enter(&w, *figure.default_group);

  char *filename = basename(argv[0]);
  Camera cam = camera(SIZEX, SIZEY, M_PI / 3);
  cam.transform =
      view_transform(POINT(2.5, 1.1, 2), POINT(0, 1., 0), VECTOR(0, 1, 0));

  /* int frame = 0; */
  /* for (float i = 0; i < M_PI * 2; i += .1) { */
  /*   Point from = POINT(sin(i) * 3, sin(i) + 1.25, cos(i) * 3); */
  /*   cam.transform = view_transform(from, POINT(0, 1.5, 0), VECTOR(0, 1, 0));
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
