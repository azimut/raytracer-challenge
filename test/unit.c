#include "../lib/camera.h"
#include "../lib/canvas.h"
#include "../lib/matrix.h"
#include "../lib/raycasting.h"
#include "../lib/shading.h"
#include "../lib/transformation.h"
#include "../lib/util.h"
#include "../lib/world.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

void test_tuple() {
  assert(tuple_equal(tuple_add(tuple(3, -2, 5, 1), tuple(-2, 3, 1, 0)),
                     tuple(1, 1, 6, 1)));
  assert(tuple_equal(tuple_sub(point(3, 2, 1), point(5, 6, 7)),
                     vector(-2, -4, -6)));
  assert(tuple_equal(tuple_sub(point(3, 2, 1), vector(5, 6, 7)),
                     point(-2, -4, -6)));
  assert(tuple_equal(tuple_sub(vector(3, 2, 1), vector(5, 6, 7)),
                     vector(-2, -4, -6)));
  assert(tuple_equal(tuple_neg(tuple(1, -2, 3, -4)), tuple(-1, 2, -3, 4)));
  assert(tuple_equal(tuple_smul(tuple(1, -2, 3, -4), 3.5),
                     tuple(3.5, -7, 10.5, -14)));
  assert(tuple_equal(tuple_smul(tuple(1, -2, 3, -4), 0.5),
                     tuple(0.5, -1, 1.5, -2)));
  assert(
      tuple_equal(tuple_sdiv(tuple(1, -2, 3, -4), 2), tuple(0.5, -1, 1.5, -2)));
  assert(1 == tuple_length(vector(1, 0, 0)));
  assert(1 == tuple_length(vector(0, 1, 0)));
  assert(1 == tuple_length(vector(0, 0, 1)));
  assert(sqrtf(14) == tuple_length(vector(1, 2, 3)));
  assert(sqrtf(14) == tuple_length(vector(-1, -2, -3)));
  assert(tuple_equal(tuple_normalize(vector(4, 0, 0)), vector(1, 0, 0)));
  assert(tuple_equal(tuple_normalize(vector(1, 2, 3)),
                     vector(0.26726, 0.53452, 0.80178)));
  assert(near(1, tuple_length(tuple_normalize(vector(1, 2, 3)))));
  assert(near(20, tuple_dot_product(vector(1, 2, 3), vector(2, 3, 4))));
  assert(tuple_equal(tuple_cross_product(vector(1, 2, 3), vector(2, 3, 4)),
                     vector(-1, 2, -1)));
  assert(tuple_equal(tuple_cross_product(vector(2, 3, 4), vector(1, 2, 3)),
                     vector(1, -2, 1)));
}

void test_canvas() {
  size_t canvas_pixels = 3;
  Canvas c = canvas(canvas_pixels, canvas_pixels);
  canvas_print(c);
  canvas_free(&c);
}

void test_matrix() {
  assert(m4_equal(m4_identity(), m4_identity()));
  assert(m3_equal(m4_submatrix(m4_identity(), 0, 0), m3_identity()));
  assert(m3_equal(m4_submatrix(m4_identity(), 3, 3), m3_identity()));
  assert(m2_equal(m2_identity(), m3_submatrix(m3_identity(), 0, 0)));
  Mat4 am4 = m4(1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2);
  assert(m4_equal(
      m4_mul(am4, m4(-2, 1, 2, 3, 3, 2, 1, -1, 4, 3, 6, 5, 1, 2, 7, 8)),
      m4(20, 22, 50, 48, 44, 54, 114, 108, 40, 58, 110, 102, 16, 26, 46, 42)));
  assert(m4_equal(m4_mul(am4, m4_identity()), am4));
  assert(tuple_equal(m4_tmul(m4(1, 2, 3, 4, 2, 4, 4, 2, 8, 6, 4, 1, 0, 0, 0, 1),
                             tuple(1, 2, 3, 1)),
                     tuple(18, 24, 33, 1)));
  assert(
      m4_equal(m4_transpose(m4(0, 9, 3, 0, 9, 8, 0, 8, 1, 8, 5, 3, 0, 0, 5, 8)),
               m4(0, 9, 1, 0, 9, 8, 8, 0, 3, 0, 5, 5, 0, 8, 3, 8)));
  /* assert(17 == m2_determinant(m2(1, 5, -3, 2))); */
  assert(m2_equal(m3_submatrix(m3(1, 5, 0, -3, 2, 7, 0, 6, -3), 0, 2),
                  m2(-3, 2, 0, 6)));
  assert(m3_equal(
      m4_submatrix(m4(-6, 1, 1, 6, -8, 5, 8, 6, -1, 0, 8, 2, -7, 1, -1, 1), 2,
                   1),
      m3(-6, 1, 6, -8, 8, 6, -7, -1, 1)));
  /* Mat3 am3 = m3(3, 5, 0, 2, -1, -7, 6, -1, 5); */
  /* assert(25 == m2_determinant(m3_submatrix(am3, 1, 0))); */
  /* assert(25 == m3_minor(am3, 1, 0)); */
  /* assert(-12 == m3_minor(am3, 0, 0)); */
  /* assert(-12 == m3_cofactor(am3, 0, 0)); */
  /* assert(25 == m3_minor(am3, 1, 0)); */
  /* assert(-25 == m3_cofactor(am3, 1, 0)); */
  /* assert(56 == m3_cofactor(m3(1, 2, 6, -5, 8, -4, 2, 6, 4), 0, 0)); */
  /* assert(12 == m3_cofactor(m3(1, 2, 6, -5, 8, -4, 2, 6, 4), 0, 1)); */
  /* assert(-46 == m3_cofactor(m3(1, 2, 6, -5, 8, -4, 2, 6, 4), 0, 2)); */
  /* assert(-196 == m3_determinant(m3(1, 2, 6, -5, 8, -4, 2, 6, 4))); */

  /* Mat4 cm4 = m4(-2, -8, 3, 5, -3, 1, 7, 3, 1, 2, -9, 6, -6, 7, 7, -9); */
  /* assert(690 == m4_cofactor(cm4, 0, 0)); */
  /* assert(447 == m4_cofactor(cm4, 0, 1)); */
  /* assert(210 == m4_cofactor(cm4, 0, 2)); */
  /* assert(51 == m4_cofactor(cm4, 0, 3)); */
  /* assert(-4071 == m4_determinant(cm4)); */

  /* Mat4 m4i = m4(6, 4, 4, 4, 5, 5, 7, 6, 4, -9, 3, -7, 9, 1, 7, -6); */
  /* assert(-2120 == m4_determinant(m4i)); */
  /* assert(m4_is_invertible(m4i)); */

  /* Mat4 m4ni = m4(-4, 2, -2, -3, 9, 6, 2, 6, 0, -5, 1, -5, 0, 0, 0, 0); */
  /* assert(0 == m4_determinant(m4ni)); */
  /* assert(!m4_is_invertible(m4ni)); */

  Mat4 a4 = m4(-5, 2, 6, -8, 1, -5, 1, 8, 7, 7, -6, -7, 1, -3, 7, 4);
  Mat4 a4i = m4(0.21805, 0.45113, 0.24060, -0.04511, -0.80827, -1.45677,
                -0.44361, 0.52068, -0.07895, -0.22368, -0.05263, 0.19737,
                -0.52256, -0.81391, -0.30075, 0.30639);
  /* assert(532 == m4_determinant(a4)); */
  /* assert(-160 == m4_cofactor(a4, 2, 3)); */
  /* assert(105 == m4_cofactor(a4, 3, 2)); */
  assert(m4_equal(m4_inverse(a4), a4i));
}

void test_transformation() {
  Mat4 t = translation(5, -3, 2);
  assert(tuple_equal(m4_tmul(t, point(-3, 4, 5)), point(2, 1, 7)));
  assert(tuple_equal(m4_tmul(m4_inverse(t), point(-3, 4, 5)), point(-8, 7, 3)));
  assert(tuple_equal(m4_tmul(t, vector(-3, 4, 5)), vector(-3, 4, 5)));

  Mat4 s = scaling(2, 3, 4);
  assert(tuple_equal(m4_tmul(s, point(-4, 6, 8)), point(-8, 18, 32)));
  assert(tuple_equal(m4_tmul(s, vector(-4, 6, 8)), vector(-8, 18, 32)));
  assert(
      tuple_equal(m4_tmul(m4_inverse(s), vector(-4, 6, 8)), vector(-2, 2, 2)));
  assert(
      tuple_equal(m4_tmul(scaling(-1, 1, 1), point(2, 3, 4)), point(-2, 3, 4)));
  // rotation_x()
  assert(tuple_equal(m4_tmul(rotation_x(M_PI / 4), point(0, 1, 0)),
                     point(0, sqrtf(2) / 2, sqrtf(2) / 2)));
  assert(tuple_equal(m4_tmul(rotation_x(M_PI / 2), point(0, 1, 0)),
                     point(0, 0, 1)));
  assert(tuple_equal(m4_tmul(m4_inverse(rotation_x(M_PI / 4)), point(0, 1, 0)),
                     point(0, sqrtf(2) / 2, -sqrtf(2) / 2)));
  // rotation_y()
  assert(tuple_equal(m4_tmul(rotation_y(M_PI / 4), point(0, 0, 1)),
                     point(sqrtf(2) / 2, 0, sqrtf(2) / 2)));
  assert(tuple_equal(m4_tmul(rotation_y(M_PI / 2), point(0, 0, 1)),
                     point(1, 0, 0)));
  // rotation_z()
  assert(tuple_equal(m4_tmul(rotation_z(M_PI / 4), point(0, 1, 0)),
                     point(-sqrtf(2) / 2, sqrtf(2) / 2, 0)));
  assert(tuple_equal(m4_tmul(rotation_z(M_PI / 2), point(0, 1, 0)),
                     point(-1, 0, 0)));
  // shearing()
  assert(tuple_equal(m4_tmul(shearing(0, 1, 0, 0, 0, 0), point(2, 3, 4)),
                     point(6, 3, 4)));
  assert(tuple_equal(m4_tmul(shearing(0, 0, 1, 0, 0, 0), point(2, 3, 4)),
                     point(2, 5, 4)));
  assert(tuple_equal(m4_tmul(shearing(0, 0, 0, 1, 0, 0), point(2, 3, 4)),
                     point(2, 7, 4)));
  assert(tuple_equal(m4_tmul(shearing(0, 0, 0, 0, 1, 0), point(2, 3, 4)),
                     point(2, 3, 6)));
  assert(tuple_equal(m4_tmul(shearing(0, 0, 0, 0, 0, 1), point(2, 3, 4)),
                     point(2, 3, 7)));
}

void test_raycasting(void) {
  Point origin = point(2, 3, 4);
  Vector direction = vector(1, 0, 0);
  Ray r = ray(origin, direction);
  assert(tuple_equal(r.origin, origin));
  assert(tuple_equal(r.direction, direction));
  assert(tuple_equal(position(r, 0), point(2, 3, 4)));
  assert(tuple_equal(position(r, 1), point(3, 3, 4)));
  assert(tuple_equal(position(r, -1), point(1, 3, 4)));
  assert(tuple_equal(position(r, 2.5), point(4.5, 3, 4)));
  Sphere s = sphere();
  r = ray(point(0, 0, -5), vector(0, 0, 1)); // ray/sphere 2 points
  Intersections si = intersect(s, r);
  assert(si.count == 2 && si.hits[0].t == 4 && si.hits[1].t == 6);
  r = ray(point(0, 1, -5), vector(0, 0, 1)); // tangent intersection
  free_intersections(&si);
  si = intersect(s, r);
  assert(si.count == 2 && si.hits[0].t == 5 && si.hits[1].t == 5);
  r = ray(point(0, 2, -5), vector(0, 0, 1)); // ray misses the sphere
  free_intersections(&si);
  si = intersect(s, r);
  assert(si.count == 0);
  r = ray(point(0, 0, 0), vector(0, 0, 1)); // ray inside sphere
  free_intersections(&si);
  si = intersect(s, r);
  assert(si.count == 2 && si.hits[0].t == -1 && si.hits[1].t == 1);
  r = ray(point(0, 0, 5), vector(0, 0, 1)); // sphere behind a ray
  free_intersections(&si);
  si = intersect(s, r);
  assert(si.count == 2 && si.hits[0].t == -6 && si.hits[1].t == -4);
  free_intersections(&si);
  // hit()
  Intersections is = intersections(s, 2, 1.0, 2.0); // all positive
  assert(hit(is) == &is.hits[0]);
  free_intersections(&is);
  is = intersections(s, 2, -1.0, 1.0); // some are negative
  assert(hit(is) == &is.hits[1]);
  free_intersections(&is);
  is = intersections(s, 2, -2.0, -1.0); // all are negative
  assert(hit(is) == NULL);
  free_intersections(&is);
  is = intersections(s, 4, 5.0, 7.0, -3.0, 2.0); // lowest non-negative
  assert(hit(is) == &is.hits[3]);
  free_intersections(&is);
  // transform()
  r = ray(point(1, 2, 3), vector(0, 1, 0));
  Mat4 m4 = translation(3, 4, 5);
  Ray new_ray = transform(r, m4);
  assert(tuple_equal(new_ray.direction, vector(0, 1, 0)));
  assert(tuple_equal(new_ray.origin, point(4, 6, 8)));
  m4 = scaling(2, 3, 4);
  new_ray = transform(r, m4);
  assert(tuple_equal(new_ray.origin, point(2, 6, 12)));
  assert(tuple_equal(new_ray.direction, vector(0, 3, 0)));
  // set_transform
  assert(m4_equal(s.transformation, m4_identity()));
  set_transform(&s, translation(2, 3, 4));
  assert(m4_equal(s.transformation, translation(2, 3, 4)));
  // transform() on intersect()
  r = ray(point(0, 0, -5), vector(0, 0, 1));
  set_transform(&s, scaling(2, 2, 2));
  is = intersect(s, r);
  assert(is.count == 2);
  assert(near(is.hits[0].t, 3));
  assert(near(is.hits[1].t, 7));
  set_transform(&s, translation(5, 0, 0));
  free_intersections(&is);
  is = intersect(s, r);
  assert(is.count == 0);
}

void test_shading(void) {
  Sphere s = sphere();
  assert(tuple_equal(vector(1, 0, 0), normal_at(s, point(1, 0, 0))));
  assert(tuple_equal(vector(0, 1, 0), normal_at(s, point(0, 1, 0))));
  assert(tuple_equal(vector(0, 0, 1), normal_at(s, point(0, 0, 1))));
  Vector nv = normal_at(s, point(sqrtf(3) / 3, sqrtf(3) / 3, sqrtf(3) / 3));
  assert(tuple_equal(vector(sqrtf(3) / 3, sqrtf(3) / 3, sqrtf(3) / 3), nv));
  assert(tuple_equal(nv, tuple_normalize(nv)));
  s = sphere();
  set_transform(&s, translation(0, 1, 0));
  assert(tuple_equal(normal_at(s, point(0, 1.70711, -0.70711)),
                     vector(0, 0.70711, -0.70711)));
  s = sphere();
  set_transform(&s, m4_mul(scaling(1, 0.5, 1), rotation_z(M_PI / 5)));
  assert(tuple_equal(normal_at(s, point(0, sqrtf(2) / 2, -sqrtf(2) / 2)),
                     vector(0, 0.97014, -0.24254)));
  // reflection()
  assert(
      tuple_equal(reflect(vector(1, -1, 0), vector(0, 1, 0)), vector(1, 1, 0)));
  assert(tuple_equal(
      reflect(vector(0, -1, 0), vector(sqrtf(2) / 2, sqrtf(2) / 2, 0)),
      vector(1, 0, 0)));
  // lighting()
  MaterialPhong m = material();
  Point position = point(0, 0, 0);
  Vector eye = vector(0, 0, -1);
  Vector normal = vector(0, 0, -1);
  PointLight light = pointlight(point(0, 0, -10), color(1, 1, 1));
  Color result = lighting(m, position, light, eye, normal, false);
  assert(color_equal(result, color(1.9, 1.9, 1.9)));

  eye = vector(0, sqrtf(2) / 2, -sqrtf(2) / 2);
  result = lighting(m, position, light, eye, normal, false);
  assert(color_equal(result, color(1, 1, 1)));

  eye = vector(0, 0, -1);
  light = pointlight(point(0, 10, -10), color(1, 1, 1));
  result = lighting(m, position, light, eye, normal, false);
  assert(color_equal(result, color(0.7364, 0.7364, 0.7364)));

  eye = vector(0, -sqrtf(2) / 2, -sqrtf(2) / 2);
  result = lighting(m, position, light, eye, normal, false);
  assert(color_equal(result, color(1.6364, 1.6364, 1.6364)));

  eye = vector(0, 0, -1);
  light = pointlight(point(0, 0, 10), color(1, 1, 1));
  result = lighting(m, position, light, eye, normal, false);
  assert(color_equal(result, color(0.1, 0.1, 0.1)));
}

void test_world(void) {
  Mat4 mat4;
  World w = {0};
  Ray r = ray(point(0, 0, -5), vector(0, 0, 1));
  Intersections is = {0};
  Color c = {0};
  w = world_default();
  r = ray(point(0, 0, -5), vector(0, 0, 1));
  is = world_intersect(w, r);
  assert(is.count == 4);
  assert(is.hits[0].t == 4.0);
  assert(is.hits[1].t == 4.5);
  assert(is.hits[2].t == 5.5);
  assert(is.hits[3].t == 6.0);
  free_intersections(&is);
  world_free(&w);
  // prepare_computations()
  Sphere s = sphere();
  Intersection i = intersection(4, s);
  r = ray(point(0, 0, -5), vector(0, 0, 1));
  Computations comp = prepare_computations(i, r);
  assert(!comp.is_inside);
  assert(comp.t == i.t);
  assert(comp.object.id == s.id);
  assert(tuple_equal(comp.point, point(0, 0, -1)));
  assert(tuple_equal(comp.eye, vector(0, 0, -1)));
  assert(tuple_equal(comp.normal, vector(0, 0, -1)));
  r = ray(point(0, 0, 0), vector(0, 0, 1));
  i = intersection(1, s);
  comp = prepare_computations(i, r);
  assert(comp.is_inside);
  assert(tuple_equal(comp.point, point(0, 0, 1)));
  assert(tuple_equal(comp.eye, vector(0, 0, -1)));
  assert(tuple_equal(comp.normal, vector(0, 0, -1)));
  // shade_color()
  r = ray(point(0, 0, -5), vector(0, 0, 1));
  w = world_default();
  i = intersection(4, w.objects[0]);
  comp = prepare_computations(i, r);
  c = shade_hit(w, comp);
  color_print(c);
  assert(color_equal(c, color(0.38066, 0.47583, 0.2855)));
  world_free(&w);
  // shade_color() - inside
  w = world_default();
  w.light = pointlight(point(0, 0.25, 0), color(1, 1, 1));
  r = ray(point(0, 0, 0), vector(0, 0, 1));
  i = intersection(0.5, w.objects[1]);
  comp = prepare_computations(i, r);
  c = shade_hit(w, comp);
  color_print(c);
  assert(color_equal(c, color(0.90498, 0.90498, 0.90498)));
  world_free(&w);
  // color_at()
  w = world_default();
  r = ray(point(0, 0, -5), vector(0, 1, 0));
  c = color_at(w, r);
  assert(color_equal(c, color(0, 0, 0))); // misses
  world_free(&w);
  w = world_default();
  r = ray(point(0, 0, -5), vector(0, 0, 1));
  c = color_at(w, r);
  assert(color_equal(c, color(0.38066, 0.47583, 0.2855))); // hits
  world_free(&w);
  w = world_default();
  w.objects[0].material.ambient = 1;
  w.objects[1].material.ambient = 1;
  r = ray(point(0, 0, 0.75), vector(0, 0, -1));
  c = color_at(w, r);
  assert(color_equal(c, w.objects[1].material.color)); // behind the ray
  world_free(&w);
  // view_transform()
  mat4 = view_transform(point(0, 0, 0), point(0, 0, -1), vector(0, 1, 0));
  assert(m4_equal(mat4, m4_identity())); // default orientation
  mat4 = view_transform(point(0, 0, 0), point(0, 0, 1), vector(0, 1, 0));
  assert(m4_equal(mat4, scaling(-1, 1, -1))); // positive z == mirror
  mat4 = view_transform(point(0, 0, 8), point(0, 0, 0), vector(0, 1, 0));
  assert(m4_equal(mat4, translation(0, 0, -8))); // proff: the world moves
  mat4 = view_transform(point(1, 3, 2), point(4, -2, 8), vector(1, 1, 0));
  assert(m4_equal(mat4, m4(-0.50709, 0.50709, 0.67612, -2.36643, 0.76772,
                           0.60609, 0.12122, -2.82843, -0.35857, 0.59761,
                           -0.71714, 0, 0, 0, 0, 1)));
  // camera()
  Camera cam = camera(160, 120, M_PI_2);
  assert(m4_equal(cam.transform, m4_identity()));
  cam = camera(200, 125, M_PI_2);
  assert(near(cam.pixel_size, 0.01)); // horizontal canvas
  cam = camera(125, 200, M_PI_2);
  assert(near(cam.pixel_size, 0.01)); // vertical canvas
  // ray_for_pixel()
  cam = camera(201, 101, M_PI_2);
  r = ray_for_pixel(cam, 100, 50);
  assert(tuple_equal(r.origin, point(0, 0, 0))); // center of canvas
  assert(tuple_equal(r.direction, vector(0, 0, -1)));
  cam = camera(201, 101, M_PI_2);
  r = ray_for_pixel(cam, 0, 0);
  assert(tuple_equal(r.origin, point(0, 0, 0))); // corner of canvas
  assert(tuple_equal(r.direction, vector(0.66519, 0.33259, -0.66851)));
  cam = camera(201, 101, M_PI_2);
  cam.transform = m4_mul(rotation_y(M_PI_4), translation(0, -2, 5));
  r = ray_for_pixel(cam, 100, 50);
  assert(tuple_equal(r.origin, point(0, 2, -5)));
  assert(tuple_equal(r.direction, vector(sqrtf(2) / 2, 0, -sqrtf(2) / 2)));
  // render()
  w = world_default();
  cam = camera(11, 11, M_PI_2);
  cam.transform =
      view_transform(point(0, 0, -5), point(0, 0, 0), vector(0, 1, 0));
  Canvas image = render(cam, w);
  assert(color_equal(canvas_get(image, 5, 5), color(0.38066, 0.47583, 0.2855)));
  world_free(&w);
  canvas_free(&image);
}

void test_shadow(void) {
  // lighting/in_shadow
  Vector eye = vector(0, 0, -1);
  Vector normal = vector(0, 0, -1);
  PointLight light = pointlight(point(0, 0, -10), color(1, 1, 1));
  bool in_shadow = true;
  MaterialPhong m = material();
  Point position = point(0, 0, 0);
  Color result = lighting(m, position, light, eye, normal, in_shadow);
  assert(color_equal(result, color(0.1, 0.1, 0.1)));
  // is_shadowed()
  World w = world_default();
  Point p = point(0, 10, 0);
  assert(!is_shadowed(w, p));
  assert(!is_shadowed(w, point(0, 10, 0)));     // nothing hit
  assert(is_shadowed(w, point(10, -10, 10)));   // nothing hit, behind sphere
  assert(!is_shadowed(w, point(-20, 20, -20))); // nothing hit, sides apart
  assert(!is_shadowed(w, point(-2, 2, -2)));    // nothing hit, in between
  world_free(&w);
  // shade_hit() given a shadow
  w = world_default();
  w.light = pointlight(point(0, 0, -10), color(1, 1, 1));
  Sphere s1 = sphere(), s2 = sphere();
  set_transform(&s2, translation(0, 0, 10));
  world_enter(&w, s1);
  world_enter(&w, s2);
  Ray r = ray(point(0, 0, 5), vector(0, 0, 1));
  Intersection i = intersection(4, s2);
  Computations comp = prepare_computations(i, r);
  Color c = shade_hit(w, comp);
  assert(color_equal(c, color(0.1, 0.1, 0.1)));
  // a hit should offset the point
  r = ray(point(0, 0, -5), vector(0, 0, 1));
  s1 = sphere();
  set_transform(&s1, translation(0, 0, 1));
  i = intersection(5, s1);
  comp = prepare_computations(i, r);
  assert(comp.over_point.z < -EPSILON / 2);
  assert(comp.point.z > comp.over_point.z);
  world_free(&w);
}

int main(void) {
  test_tuple();
  test_canvas();
  test_matrix();
  test_transformation();
  test_raycasting();
  test_shading();
  test_world();
  test_shadow();
  printf("ALL OK!\n");
  return 0;
}
