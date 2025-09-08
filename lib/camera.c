#include "./camera.h"
#include "./transformation.h"
#include <assert.h>
#include <math.h>

Mat4 view_transform(Point from, Point to, Vector up) {
#ifndef BLAZE
  assert(is_point(from) && is_point(to) && is_vector(up));
#endif
  Vector forward = tuple_normalize(tuple_sub(to, from));
  Vector left = tuple_cross_product(forward, tuple_normalize(up));
  Vector true_up = tuple_cross_product(left, forward);
  Mat4 orientation =
      m4(left.x, left.y, left.z, 0, true_up.x, true_up.y, true_up.z, 0,
         -forward.x, -forward.y, -forward.z, 0, 0, 0, 0, 1);
  return m4_mul(orientation, translation(-from.x, -from.y, -from.z));
}

Camera camera(size_t hsize, size_t vsize, double fov) {
  double half_view = tan(fov / 2); // width of half of canvas
  double aspect = hsize / (double)vsize;
  double half_width, half_height;
  if (aspect >= 1) {
    half_width = half_view;
    half_height = half_view / aspect;
  } else {
    half_width = half_view * aspect;
    half_height = half_view;
  }
  return (Camera){
      .half_height = half_height,
      .half_width = half_width,
      .pixel_size = (half_width * 2) / hsize,
      .transform = M4_IDENTITY,
      .hsize = hsize,
      .vsize = vsize,
      .fov = fov,
  };
}

// Computes the world coordinates at the center of the given pixel coords
// and then construct a Ray that passes through that point.
Ray ray_for_pixel(const Camera cam, double px, double py) {
  // ?offset from the edge of the canvas to the pixel's center
  const double xoffset = (px + 0.5) * cam.pixel_size;
  const double yoffset = (py + 0.5) * cam.pixel_size;
  // untransformed pixel coord in world space
  const double world_x = cam.half_width - xoffset;
  const double world_y = cam.half_height - yoffset;
  // transform canvas_point, and origin
  const Point pixel =
      m4_tmul(m4_inverse(cam.transform), POINT(world_x, world_y, -1));
  const Point origin = m4_tmul(m4_inverse(cam.transform), POINT(0, 0, 0));
  const Vector direction = tuple_normalize(tuple_sub(pixel, origin));
  return (Ray){
      .origin = origin,
      .direction = direction,
  };
}

Canvas render(const Camera cam, const World world) {
  Canvas c = canvas(cam.hsize, cam.vsize);
  for (double row = 0; row < cam.vsize; ++row) {
    for (double col = 0; col < cam.hsize; ++col) {
      Ray ray = ray_for_pixel(cam, col, row);
      Color pixel = color_at(world, ray, REFLECTION_HITS);
      if (NSAMPLES > 1) { // ANTIALIASING
        for (size_t i = 1; i < NSAMPLES; ++i) {
          ray = ray_for_pixel(cam, col + drand48(), row + drand48());
          pixel = color_add(pixel, color_at(world, ray, REFLECTION_HITS));
        }
        pixel = color_sdiv(pixel, NSAMPLES);
      }
      canvas_set(&c, row, col, pixel);
    }
  }
  return c;
}
