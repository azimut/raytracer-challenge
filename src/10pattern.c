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

int main(int argc, char *argv[]) {
  (void)argc;
  World w = {0};
  w.light = pointlight(point(9, 3, 4), color(1, 0.9450981, 0.87843144));

  Camera cam = camera(200, 200, M_PI / 3);
  cam.transform =
      view_transform(point(3, 0.5, 3), point(-1, 1.5, -1.5), vector(0, 1, 0));

  Shape floor = plane();
  floor.material = material();
  floor.material.pattern = pattern_checkers(WHITE, BLACK);
  /* floor.material.pattern.transformation = scaling(0.25, 1, 1); */
  floor.material.color = color(.4, 0.9, 0.9);
  floor.material.specular = 0;
  world_enter(&w, floor);

  double radius = 50;
  double rot = 0;
  for (size_t i = 0; i < 6; ++i) {
    Shape hexa = plane();
    hexa.transformation =
        m4_mul(translation(radius * cos(M_PI / 6.0 + (i * M_PI) / 3.0), 0,
                           radius * sin(M_PI / 6.0 + (i * M_PI) / 3.0)),
               m4_mul(rotation_y(radians(rot)), rotation_x(M_PI / 2)));
    rot -= 60;
    hexa.material.color = color(136.0 / 255, 198.0 / 255, 252.0 / 255);
    world_enter(&w, hexa);
  }

  Shape roof = plane();
  roof.transformation = translation(0, 20, 0);
  roof.material.pattern = pattern_rings(WHITE, BLACK);
  roof.material.pattern.transformation =
      m4_mul(translation(0, 0, 0), scaling(0.5, 1, 0.5));
  roof.material.color = color(136.0 / 255, 198.0 / 255, 252.0 / 255);
  world_enter(&w, roof);

  Shape middle = sphere();
  middle.transformation = translation(-0.5, 1, 0.5);
  middle.material = material();
  middle.material.color = color(0.1, 1, 0.5);
  middle.material.pattern = pattern_stripes(WHITE, BLACK);
  middle.material.pattern.transformation =
      m4_mul(rotation_z(M_PI / 3), scaling(0.25, 1, 1));
  middle.material.diffuse = 0.7;
  middle.material.specular = 0.3;
  world_enter(&w, middle);

  Shape right = sphere();
  right.transformation =
      m4_mul(translation(1.5, 0.5, -0.5), scaling(0.5, 0.5, 0.5));
  right.material = material();
  right.material.pattern = pattern_gradient(RED, BLUE);
  right.material.pattern.transformation =
      m4_mul(translation(1, 0, 0), scaling(2, 1, 1));
  right.material.color = color(0.5, 1, 0.1);
  right.material.diffuse = 0.7;
  right.material.specular = 0.3;
  world_enter(&w, right);

  char *buff = calloc(100, sizeof(char));
  strcat(strcat(strcat(buff, "media/"), basename(argv[0])), ".ppm");
  Canvas canvas = render(cam, w);
  canvas_save(canvas, buff);
  canvas_free(&canvas);
  world_free(&w);
  free(buff);
  return 0;
}
