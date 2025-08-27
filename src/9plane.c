#include "../lib/camera.h"
#include "../lib/canvas.h"
#include "../lib/color.h"
#include "../lib/material.h"
#include "../lib/raycasting.h"
#include "../lib/shading.h"
#include "../lib/transformation.h"
#include "../lib/tuple.h"
#include "../lib/world.h"
#include <math.h>

int main(void) {
  World w = {0};
  w.light = pointlight(point(-10, 4, -10), color(1, 0.9450981, 0.87843144));

  Camera cam = camera(500, 500, M_PI / 2);
  cam.transform =
      view_transform(point(0, 1.5, -5), point(0, 1, 0), vector(0, 1, 0));

  Shape floor = plane();
  floor.material = material();
  floor.material.color = color(1, 0.9, 0.9);
  floor.material.specular = 0;
  world_enter(&w, floor);

  Shape middle = sphere();
  middle.transformation = translation(-0.5, 1, 0.5);
  middle.material = material();
  middle.material.color = color(0.1, 1, 0.5);
  middle.material.diffuse = 0.7;
  middle.material.specular = 0.3;
  world_enter(&w, middle);

  Shape right = sphere();
  right.transformation =
      m4_mul(translation(1.5, 0.5, -0.5), scaling(0.5, 0.5, 0.5));
  right.material = material();
  right.material.color = color(0.5, 1, 0.1);
  right.material.diffuse = 0.7;
  right.material.specular = 0.3;
  world_enter(&w, right);

  Shape left = sphere();
  left.transformation =
      m4_mul(translation(-1.5, 0.33, -0.75), scaling(0.33, 0.33, 0.33));
  left.material = material();
  left.material.color = color(1, 0.8, 0.1);
  left.material.diffuse = 0.7;
  left.material.specular = 0.3;
  world_enter(&w, left);

  Canvas canvas = render(cam, w);
  canvas_save(canvas, "media/9plane.ppm");
  canvas_free(&canvas);
  world_free(&w);
  return 0;
}
