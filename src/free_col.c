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

#ifndef AREA_SAMPLES
#define AREA_SAMPLES 4
#endif

int main(int argc, char *argv[]) {
  (void)argc;
  World w = {0};

  Light p = {0};
  p = pointlight(POINT(15, 7, -15), HEX2COLOR("#d601ff"));
  p.attenuation = LIGHT_SIZE_32;
  world_enlight(&w, p);
  /* p = pointlight(POINT(3, 12, -7), HEX2COLOR("#01ff9f")); */
  /* p.attenuation = LIGHT_SIZE_100; */
  /* world_enlight(&w, p); */

  /* p = arealight(POINT(0, 7, 0), VECTOR(0, 0, 2), AREA_SAMPLES, VECTOR(2, 0,
   * 0), */
  /*               AREA_SAMPLES, HEX2COLOR("#d601ff")); */
  /* p.attenuation = LIGHT_SIZE_INFINITE; */
  /* world_enlight(&w, p); */
  /* p = arealight(POINT(3, 12, -7), VECTOR(2, 0, 0), AREA_SAMPLES, */
  /*               VECTOR(0, 2, 0), AREA_SAMPLES, HEX2COLOR("#01ff9f")); */
  /* p.attenuation = LIGHT_SIZE_200; */
  /* world_enlight(&w, p); */

  p = arealight(POINT(-3, 2, -7), VECTOR(0, 0, 2), AREA_SAMPLES,
                VECTOR(0, 2, 0), AREA_SAMPLES, HEX2COLOR("#01ff9f"));
  p.attenuation = LIGHT_SIZE_200;
  world_enlight(&w, p);

  /* p = pointlight(POINT(-10, 0, 0), HEX2COLOR("#01ff9f")); */
  /* p.attenuation = LIGHT_SIZE_50; */
  /* world_enlight(&w, p); */

  double angle = 0;
  for (size_t i = 0; i < 3; ++i) {
    Shape p = plane();
    p.material.pattern = pattern_stripes(BLACK, WHITE);
    p.material.pattern.transformation =
        m4_mul(scaling(1, 1, 11), rotation_y(radians(90)));
    p.transformation =
        m4_mul(rotation_x(radians(angle)), translation(0, -150, 0));
    world_enter(&w, p);
    angle += 120;
  }

  Shape s = cylinder();
  s.transformation =
      m4_mul(translation(7, 0, 0),
             m4_mul(rotation_x(radians(20)), rotation_z(radians(-40))));
  s.material.diffuse = 0.5;
  s.material.ambient = AMBIENT;
  s.shape_data.cylinder.minimum = -2;
  s.shape_data.cylinder.maximum = +2;
  s.shape_data.cylinder.closed = true;
  s.shadowless = true;
  /* s.material.reflective = 0.3; */
  /* s.material.transparency = 1; */
  /* s.material.refractive_index = DEFAULT_REFRACTIVE_GLASS; */
  world_enter(&w, s);

  char *filename = basename(argv[0]);
  Camera cam = camera(SIZEX, SIZEY, M_PI / 2);
  cam.transform =
      view_transform(POINT(0, 0, 0), POINT(1, 0, 0), VECTOR(0, 1, 0));

  /* int frame = 0; */
  /* for (float i = 0; i < M_PI * 2; i += .1) { */
  /*   /\* Point from = POINT(sin(i) * 3, 1.5, cos(i) * 3); *\/ */
  /*   Point from = POINT(0, 0, 0); */
  /*   cam.transform = */
  /*       view_transform(from, POINT(cos(i) * 3, 0, sin(i) * 3), VECTOR(0, 1,
   * 0)); */
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
