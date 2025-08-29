#ifndef CAMERA_H
#define CAMERA_H

#include "./canvas.h"
#include "./matrix.h"
#include "./raycasting.h"
#include "./world.h"

#define REFLECTION_HITS 5

typedef struct Camera {
  size_t hsize, vsize; // in pixels
  double fov;          // an angle
  double pixel_size;   // of canvas, at 1 unit distance
  double half_width, half_height;
  Mat4 transform;
} Camera;

Mat4 view_transform(Point, Point, Vector);
Camera camera(size_t, size_t, double);
Ray ray_for_pixel(Camera, size_t, size_t);
Canvas render(Camera, World);

#endif /* CAMERA_H */
