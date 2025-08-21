#include "./camera.h"
#include "./transformation.h"
#include <assert.h>
#include <math.h>

Mat4 view_transform(Point from, Point to, Vector up) {
  assert(is_point(from) && is_point(to) && is_vector(up));
  Vector forward = tuple_normalize(tuple_sub(to, from));
  Vector left = tuple_cross_product(forward, tuple_normalize(up));
  Vector true_up = tuple_cross_product(left, forward);
  Mat4 orientation =
      m4(left.x, left.y, left.z, 0, true_up.x, true_up.y, true_up.z, 0,
         -forward.x, -forward.y, -forward.z, 0, 0, 0, 0, 1);
  return m4_mul(orientation, translation(-from.x, -from.y, -from.z));
}

Camera camera(size_t hsize, size_t vsize, float fov) {
  float half_view = tanf(fov / 2); // width of half of canvas
  float aspect = hsize / (float)vsize;
  float half_width, half_height;
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
      .transform = m4_identity(),
      .hsize = hsize,
      .vsize = vsize,
      .fov = fov,
  };
}

// Computes the world coordinates at the center of the given pixel coords
// and then construct a Ray that passes through that point.
Ray ray_for_pixel(Camera cam, size_t px, size_t py) {
  // ?offset from the edge of the canvas to the pixel's center
  float xoffset = ((float)px + 0.5) * cam.pixel_size;
  float yoffset = ((float)py + 0.5) * cam.pixel_size;
  // untransformed pixel coord in world space
  float world_x = cam.half_width - xoffset;
  float world_y = cam.half_height - yoffset;
  // transform canvas_point, and origin
  Point pixel = m4_tmul(m4_inverse(cam.transform), point(world_x, world_y, -1));
  Point origin = m4_tmul(m4_inverse(cam.transform), point(0, 0, 0));
  Vector direction = tuple_normalize(tuple_sub(pixel, origin));
  return (Ray){
      .origin = origin,
      .direction = direction,
  };
}

Canvas render(Camera cam, World world) {
  Canvas c = canvas(cam.hsize, cam.vsize);
  Ray ray;
  for (size_t row = 0; row < cam.vsize; ++row) {
    for (size_t col = 0; col < cam.hsize; ++col) {
      ray = ray_for_pixel(cam, col, row);
      canvas_set(&c, row, col, color_at(world, ray));
    }
  }
  return c;
}
