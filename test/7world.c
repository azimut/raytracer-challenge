#include "../src/camera.h"
#include "../src/canvas.h"
#include "../src/color.h"
#include "../src/material.h"
#include "../src/raycasting.h"
#include "../src/shading.h"
#include "../src/transformation.h"
#include "../src/tuple.h"
#include "../src/world.h"
#include <math.h>

int main(void) {
  World w = {0};
  w.light = pointlight(point(-10, 10, -10), color(1, 1, 1));

  Camera cam = camera(300, 300, M_PI / 3);
  cam.transform =
      view_transform(point(0, 1.5, -5), point(0, 1, 0), vector(0, 1, 0));

  Sphere floor = sphere();
  floor.transformation = scaling(10, 0.01, 10);
  floor.material = material();
  floor.material.color = color(1, 0.9, 0.9);
  floor.material.specular = 0;
  world_enter(&w, floor);

  Sphere left_wall = sphere();
  left_wall.transformation =
      m4_mul(translation(0, 0, 5),
             m4_mul(rotation_y(-M_PI_4),
                    m4_mul(rotation_x(M_PI_2), scaling(10, 0.01, 10))));
  left_wall.material = floor.material;
  world_enter(&w, left_wall);

  Sphere right_wall = sphere();
  right_wall.transformation =
      m4_mul(translation(0, 0, 5),
             m4_mul(rotation_y(M_PI_4),
                    m4_mul(rotation_x(M_PI_2), scaling(10, 0.01, 10))));
  right_wall.material = floor.material;
  world_enter(&w, right_wall);

  Sphere middle = sphere();
  middle.transformation = translation(-0.5, 1, 0.5);
  middle.material = material();
  middle.material.color = color(0.1, 1, 0.5);
  middle.material.diffuse = 0.7;
  middle.material.specular = 0.3;
  world_enter(&w, middle);

  Sphere right = sphere();
  right.transformation =
      m4_mul(translation(1.5, 0.5, -0.5), scaling(0.5, 0.5, 0.5));
  right.material = material();
  right.material.color = color(0.5, 1, 0.1);
  right.material.diffuse = 0.7;
  right.material.specular = 0.3;
  world_enter(&w, right);

  Sphere left = sphere();
  left.transformation =
      m4_mul(translation(-1.5, 0.33, -0.75), scaling(0.33, 0.33, 0.33));
  left.material = material();
  left.material.color = color(1, 0.8, 0.1);
  left.material.diffuse = 0.7;
  left.material.specular = 0.3;
  world_enter(&w, left);

  Canvas canvas = render(cam, w);
  canvas_print(canvas);
  return 0;
}
