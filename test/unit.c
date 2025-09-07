#include "../lib/camera.h"
#include "../lib/canvas.h"
#include "../lib/matrix.h"
#include "../lib/obj.h"
#include "../lib/patterns.h"
#include "../lib/raycasting.h"
#include "../lib/shading.h"
#include "../lib/transformation.h"
#include "../lib/util.h"
#include "../lib/world.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

void test_tuple(void) {
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
  assert(sqrt(14) == tuple_length(vector(1, 2, 3)));
  assert(sqrt(14) == tuple_length(vector(-1, -2, -3)));
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

void test_canvas(void) {
  size_t canvas_pixels = 3;
  Canvas c = canvas(canvas_pixels, canvas_pixels);
  canvas_print(c);
  canvas_free(&c);
}

void test_matrix(void) {
  assert(m4_equal(M4_IDENTITY, M4_IDENTITY));
  assert(m3_equal(m4_submatrix(M4_IDENTITY, 0, 0), M3_IDENTITY));
  assert(m3_equal(m4_submatrix(M4_IDENTITY, 3, 3), M3_IDENTITY));
  assert(m2_equal(M2_IDENTITY, m3_submatrix(M3_IDENTITY, 0, 0)));
  Mat4 am4 = m4(1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2);
  assert(m4_equal(
      m4_mul(am4, m4(-2, 1, 2, 3, 3, 2, 1, -1, 4, 3, 6, 5, 1, 2, 7, 8)),
      m4(20, 22, 50, 48, 44, 54, 114, 108, 40, 58, 110, 102, 16, 26, 46, 42)));
  assert(m4_equal(m4_mul(am4, M4_IDENTITY), am4));
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

void test_transformation(void) {
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
  Shape s = sphere();
  r = ray(point(0, 0, -5), vector(0, 0, 1)); // ray/sphere 2 points
  Intersections si = intersect(s, r);
  assert(si.count == 2 && si.hits[0].t == 4 && si.hits[1].t == 6);
  r = ray(point(0, 1, -5), vector(0, 0, 1)); // tangent intersection
  intersections_free(&si);
  si = intersect(s, r);
  assert(si.count == 2 && si.hits[0].t == 5 && si.hits[1].t == 5);
  r = ray(point(0, 2, -5), vector(0, 0, 1)); // ray misses the sphere
  intersections_free(&si);
  si = intersect(s, r);
  assert(si.count == 0);
  r = ray(point(0, 0, 0), vector(0, 0, 1)); // ray inside sphere
  intersections_free(&si);
  si = intersect(s, r);
  assert(si.count == 2 && si.hits[0].t == -1 && si.hits[1].t == 1);
  r = ray(point(0, 0, 5), vector(0, 0, 1)); // sphere behind a ray
  intersections_free(&si);
  si = intersect(s, r);
  assert(si.count == 2 && si.hits[0].t == -6 && si.hits[1].t == -4);
  intersections_free(&si);
  // hit()
  Intersections is = intersections_new(4); // all positive
  intersections_insert(&is, (Intersection){1, s});
  intersections_insert(&is, (Intersection){2, s});
  assert(hit(is) == &is.hits[0]);
  intersections_free(&is);
  is = intersections_new(4); // all positive
  intersections_insert(&is, (Intersection){-1, s});
  intersections_insert(&is, (Intersection){+1, s});
  assert(hit(is) == &is.hits[1]);
  intersections_free(&is);
  is = intersections_new(4); // all are negative
  intersections_insert(&is, (Intersection){-2, s});
  intersections_insert(&is, (Intersection){-1, s});
  assert(hit(is) == NULL);
  intersections_free(&is);
  is = intersections_new(4); // lowest non-negative
  intersections_insert(&is, (Intersection){+5, s});
  intersections_insert(&is, (Intersection){+7, s});
  intersections_insert(&is, (Intersection){-3, s});
  intersections_insert(&is, (Intersection){+2, s});
  assert(hit(is) == &is.hits[3]);
  intersections_free(&is);
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
  assert(m4_equal(s.transformation, M4_IDENTITY));
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
  intersections_free(&is);
  is = intersect(s, r);
  assert(is.count == 0);
  intersections_free(&is);
}

void test_shading(void) {
  Shape s = sphere();
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
  Light light = pointlight(point(0, 0, -10), WHITE);
  Color result = lighting(m, s, position, light, eye, normal, 1);
  assert(color_equal(result, color(1.9, 1.9, 1.9)));

  eye = vector(0, sqrtf(2) / 2, -sqrtf(2) / 2);
  result = lighting(m, s, position, light, eye, normal, 1);
  assert(color_equal(result, WHITE));

  eye = vector(0, 0, -1);
  light = pointlight(point(0, 10, -10), WHITE);
  result = lighting(m, s, position, light, eye, normal, 1);
  assert(color_equal(result, color(0.7364, 0.7364, 0.7364)));

  eye = vector(0, -sqrtf(2) / 2, -sqrtf(2) / 2);
  result = lighting(m, s, position, light, eye, normal, 1);
  assert(color_equal(result, color(1.6364, 1.6364, 1.6364)));

  eye = vector(0, 0, -1);
  light = pointlight(point(0, 0, 10), WHITE);
  result = lighting(m, s, position, light, eye, normal, 1);
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
  intersections_free(&is);
  world_free(&w);
  // prepare_computations()
  Shape s = sphere();
  Intersection i = intersection(4, s);
  r = ray(point(0, 0, -5), vector(0, 0, 1));
  Computations comp = prepare_computations(i, r, is);
  assert(!comp.is_inside);
  assert(comp.t == i.t);
  assert(comp.object.id == s.id);
  assert(tuple_equal(comp.point, point(0, 0, -1)));
  assert(tuple_equal(comp.eye, vector(0, 0, -1)));
  assert(tuple_equal(comp.normal, vector(0, 0, -1)));
  r = ray(point(0, 0, 0), vector(0, 0, 1));
  i = intersection(1, s);
  comp = prepare_computations(i, r, is);
  assert(comp.is_inside);
  assert(tuple_equal(comp.point, point(0, 0, 1)));
  assert(tuple_equal(comp.eye, vector(0, 0, -1)));
  assert(tuple_equal(comp.normal, vector(0, 0, -1)));
  // shade_color()
  r = ray(point(0, 0, -5), vector(0, 0, 1));
  w = world_default();
  i = intersection(4, w.shapes.shapes[0]);
  comp = prepare_computations(i, r, is);
  c = shade_hit(w, comp, 1);
  assert(color_equal(c, color(0.38066, 0.47583, 0.2855)));
  world_free(&w);
  // shade_color() - inside
  w = world_default();
  w.lights[0].position = point(0, 0.25, 0);
  w.lights[0].intensity = WHITE;
  r = ray(point(0, 0, 0), vector(0, 0, 1));
  i = intersection(0.5, w.shapes.shapes[1]);
  comp = prepare_computations(i, r, is);
  c = shade_hit(w, comp, 1);
  assert(color_equal(c, color(0.90498, 0.90498, 0.90498)));
  world_free(&w);
  // color_at()
  w = world_default();
  r = ray(point(0, 0, -5), vector(0, 1, 0));
  c = color_at(w, r, 1);
  assert(color_equal(c, BLACK)); // misses
  world_free(&w);
  w = world_default();
  r = ray(point(0, 0, -5), vector(0, 0, 1));
  c = color_at(w, r, 1);
  assert(color_equal(c, color(0.38066, 0.47583, 0.2855))); // hits
  world_free(&w);
  w = world_default();
  w.shapes.shapes[0].material.ambient = 1;
  w.shapes.shapes[1].material.ambient = 1;
  r = ray(point(0, 0, 0.75), vector(0, 0, -1));
  c = color_at(w, r, 1);
  assert(color_equal(c, w.shapes.shapes[1].material.color)); // behind the ray
  world_free(&w);
  // view_transform()
  mat4 = view_transform(point(0, 0, 0), point(0, 0, -1), vector(0, 1, 0));
  assert(m4_equal(mat4, M4_IDENTITY)); // default orientation
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
  assert(m4_equal(cam.transform, M4_IDENTITY));
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
  Light light = pointlight(point(0, 0, -10), WHITE);
  MaterialPhong m = material();
  Point position = point(0, 0, 0);
  Shape s = sphere();
  Color result = lighting(m, s, position, light, eye, normal, 0);
  assert(color_equal(result, color(0.1, 0.1, 0.1)));
  // is_shadowed()
  World w = world_default();
  Point p = point(0, 10, 0);
  Point lp = w.lights[0].position;
  assert(!is_shadowed(w, p, lp));
  assert(!is_shadowed(w, point(0, 10, 0), lp));   // nothing hit
  assert(is_shadowed(w, point(10, -10, 10), lp)); // nothing hit, behind sphere
  assert(!is_shadowed(w, point(-20, 20, -20), lp)); // nothing hit, sides apart
  assert(!is_shadowed(w, point(-2, 2, -2), lp));    // nothing hit, in between
  world_free(&w);
  // shade_hit() given a shadow
  w = world_default();
  w.lights[0].position = point(0, 0, -10);
  w.lights[0].intensity = WHITE;
  Shape s1 = sphere(), s2 = sphere();
  set_transform(&s2, translation(0, 0, 10));
  world_enter(&w, s1);
  world_enter(&w, s2);
  Ray r = ray(point(0, 0, 5), vector(0, 0, 1));
  Intersection i = intersection(4, s2);
  Intersections is = {0};
  Computations comp = prepare_computations(i, r, is);
  Color c = shade_hit(w, comp, 1);
  assert(color_equal(c, color(0.1, 0.1, 0.1)));
  // a hit should offset the point
  r = ray(point(0, 0, -5), vector(0, 0, 1));
  s1 = sphere();
  set_transform(&s1, translation(0, 0, 1));
  i = intersection(5, s1);
  comp = prepare_computations(i, r, is);
  assert(comp.over_point.z < -EPSILON / 2);
  assert(comp.point.z > comp.over_point.z);
  world_free(&w);
}

void test_plane(void) {
  Shape p = plane();
  assert(m4_equal(p.transformation, M4_IDENTITY));
  Ray r = ray(point(0, 10, 0), vector(0, 0, 1));
  Intersections is = intersect(p, r);
  assert(is.count == 0); // ray parallel to plane
  r = ray(point(0, 0, 0), vector(0, 0, 1));
  intersections_free(&is);
  is = intersect(p, r);
  assert(is.count == 0); // coplanar ray
  r = ray(point(0, 1, 0), vector(0, -1, 0));
  intersections_free(&is);
  is = intersect(p, r);
  assert(is.count == 1); // ray from above
  assert(near(is.hits[0].t, 1));
  assert(is.hits[0].object.id == p.id);
  r = ray(point(0, -1, 0), vector(0, 1, 0));
  intersections_free(&is);
  is = intersect(p, r);
  assert(is.count == 1); // ray from below
  assert(near(is.hits[0].t, 1));
  assert(is.hits[0].object.id == p.id);
  intersections_free(&is);
}

void test_patterns(void) {
  Pattern ps = pattern_stripes(WHITE, BLACK);
  assert(color_equal(WHITE, ps.a) && color_equal(BLACK, ps.b));
  // constant in y
  assert(color_equal(pattern_at(ps, point(0, 0, 0)), WHITE));
  assert(color_equal(pattern_at(ps, point(0, 1, 0)), WHITE));
  assert(color_equal(pattern_at(ps, point(0, 2, 0)), WHITE));
  // constant in z
  assert(color_equal(pattern_at(ps, point(0, 0, 0)), WHITE));
  assert(color_equal(pattern_at(ps, point(0, 0, 1)), WHITE));
  assert(color_equal(pattern_at(ps, point(0, 0, 2)), WHITE));
  // alternating in x
  assert(color_equal(pattern_at(ps, point(+0.0, 0, 0)), WHITE));
  assert(color_equal(pattern_at(ps, point(+0.9, 0, 0)), WHITE));
  assert(color_equal(pattern_at(ps, point(+1.0, 0, 0)), BLACK));
  assert(color_equal(pattern_at(ps, point(-0.1, 0, 0)), BLACK));
  assert(color_equal(pattern_at(ps, point(-1.0, 0, 0)), BLACK));
  assert(color_equal(pattern_at(ps, point(-1.1, 0, 0)), WHITE));
  // pattern_test() - object transform
  Shape s = sphere();
  s.transformation = scaling(2, 2, 2);
  ps = pattern_test();
  Color c = pattern_at_shape(ps, s, point(2, 3, 4));
  assert(color_equal(c, color(1, 1.5, 2)));
  // pattern_test() - pattern transform
  s = sphere();
  ps = pattern_test();
  ps.transformation = scaling(2, 2, 2);
  c = pattern_at_shape(ps, s, point(2, 3, 4));
  assert(color_equal(c, color(1, 1.5, 2))); // with transform
  // pattern_test() - object+pattern transform
  s = sphere();
  s.transformation = scaling(2, 2, 2);
  ps = pattern_test();
  ps.transformation = translation(0.5, 1, 1.5);
  c = pattern_at_shape(ps, s, point(2.5, 3, 3.5));
  assert(color_equal(c, color(0.75, 0.5, 0.25))); // with transform
  // lighting()
  MaterialPhong m = material();
  m.pattern = pattern_stripes(WHITE, BLACK);
  m.ambient = 1;
  m.diffuse = 0;
  m.specular = 0;
  Vector eye = vector(0, 0, -1);
  Vector normal = vector(0, 0, -1);
  Light light = pointlight(point(0, 0, -10), WHITE);
  s = sphere();
  Color c1 = lighting(m, s, point(0.9, 0, 0), light, eye, normal, 1);
  Color c2 = lighting(m, s, point(1.1, 0, 0), light, eye, normal, 1);
  assert(color_equal(c1, WHITE));
  assert(color_equal(c2, BLACK));
  // stripes + object transform
  set_transform(&s, scaling(2, 2, 2));
  pattern_at_shape(ps, s, point(1.5, 0, 0)); // !!!!
  // stripes + pattern transform
  s = sphere();
  ps = pattern_stripes(WHITE, BLACK);
  pattern_set_transformation(&ps, scaling(2, 2, 2));
  c1 = pattern_at_shape(ps, s, point(1.5, 0, 0));
  assert(color_equal(c1, WHITE));
  // stripes + object+pattern transform
  s = sphere();
  set_transform(&s, scaling(2, 2, 2));
  ps = pattern_stripes(WHITE, BLACK);
  pattern_set_transformation(&ps, translation(0.5, 0, 0));
  c1 = pattern_at_shape(ps, s, point(2.5, 0, 0));
  assert(color_equal(c1, WHITE));
  // gradient
  ps = pattern_gradient(WHITE, BLACK);
  assert(color_equal(WHITE, pattern_at(ps, point(0, 0, 0))));
  assert(
      color_equal(color(0.75, 0.75, 0.75), pattern_at(ps, point(0.25, 0, 0))));
  assert(
      color_equal(color(0.50, 0.50, 0.50), pattern_at(ps, point(0.5, 0, 0))));
  assert(
      color_equal(color(0.25, 0.25, 0.25), pattern_at(ps, point(0.75, 0, 0))));
  // rings
  ps = pattern_rings(WHITE, BLACK);
  assert(color_equal(WHITE, pattern_at(ps, point(0, 0, 0))));
  assert(color_equal(BLACK, pattern_at(ps, point(1, 0, 0))));
  assert(color_equal(BLACK, pattern_at(ps, point(0, 0, 1))));
  assert(color_equal(BLACK, pattern_at(ps, point(0.708, 0, 0.708))));
  // checkers
  ps = pattern_checkers(WHITE, BLACK);
  // repeat in x
  assert(color_equal(WHITE, pattern_at(ps, point(0.00, 0, 0))));
  assert(color_equal(WHITE, pattern_at(ps, point(0.99, 0, 0))));
  assert(color_equal(BLACK, pattern_at(ps, point(1.01, 0, 0))));
  // repeat in y
  assert(color_equal(WHITE, pattern_at(ps, point(0, 0.00, 0))));
  assert(color_equal(WHITE, pattern_at(ps, point(0, 0.99, 0))));
  assert(color_equal(BLACK, pattern_at(ps, point(0, 1.01, 0))));
  // repeat in z
  assert(color_equal(WHITE, pattern_at(ps, point(0, 0, 0.00))));
  assert(color_equal(WHITE, pattern_at(ps, point(0, 0, 0.99))));
  assert(color_equal(BLACK, pattern_at(ps, point(0, 0, 1.01))));
}

void test_reflections(void) {
  MaterialPhong m = material();
  assert(m.reflective == 0);
  Shape pl = plane();
  Ray r = ray(point(0, 1, -1), vector(0, -sqrt(2) / 2, sqrt(2) / 2));
  Intersection i = intersection(sqrt(2), pl);
  Intersections is = {0};
  Computations comp = prepare_computations(i, r, is);
  assert(tuple_equal(comp.reflect, vector(0, sqrt(2) / 2, sqrt(2) / 2)));
  // strike a non-reflective surface
  World world = world_default();
  r = ray(point(0, 0, 0), vector(0, 0, 1));
  world.shapes.shapes[1].material.ambient = 1;
  i = intersection(1, world.shapes.shapes[1]);
  comp = prepare_computations(i, r, is);
  assert(color_equal(BLACK, reflected_color(world, comp, 1)));
  world_free(&world);
  // strike a reflective surface
  world = world_default();
  pl = plane();
  pl.material.reflective = 0.5;
  pl.transformation = translation(0, -1, 0);
  world_enter(&world, pl);
  r = ray(point(0, 0, -3), vector(0, -sqrt(2) / 2, sqrt(2) / 2));
  i = intersection(sqrt(2), pl);
  comp = prepare_computations(i, r, is);
  assert(color_equal(color(0.19032, 0.2379, 0.14274),
                     reflected_color(world, comp, 1)));
  world_free(&world);
  // shade_hit() updated
  world = world_default();
  pl = plane();
  pl.material.reflective = 0.5;
  pl.transformation = translation(0, -1, 0);
  world_enter(&world, pl);
  r = ray(point(0, 0, -3), vector(0, -sqrt(2) / 2, sqrt(2) / 2));
  i = intersection(sqrt(2), pl);
  comp = prepare_computations(i, r, is);
  assert(
      color_equal(color(0.87677, 0.92436, 0.82918), shade_hit(world, comp, 1)));
  world_free(&world);
  // mutually reflected surfaces
  world = world_default();
  world.lights[0].position = point(0, 0, 0);
  world.lights[0].intensity = WHITE;
  Shape lower = plane();
  lower.material.reflective = 1;
  lower.transformation = translation(0, -1, 0);
  world_enter(&world, lower);
  Shape upper = plane();
  lower.material.reflective = 1;
  lower.transformation = translation(0, 1, 0);
  world_enter(&world, upper);
  r = ray(point(0, 0, 0), vector(0, 1, 0));
  color_at(world, r, 1); // returns, and no loops TODO: timeout?
  world_free(&world);
  // limit recursion
  world = world_default();
  pl = plane();
  pl.material.reflective = 0.5;
  pl.transformation = translation(0, -1, 0);
  world_enter(&world, pl);
  r = ray(point(0, 0, -3), vector(0, -sqrt(2) / 2, sqrt(2) / 2));
  i = intersection(sqrt(2), pl);
  comp = prepare_computations(i, r, is);
  assert(color_equal(BLACK, reflected_color(world, comp, 0)));
  world_free(&world);
}

void test_refraction(void) {
  MaterialPhong m = material();
  assert(m.transparency == 0 && m.refractive_index == 1);
  Shape gs = sphere_glass();
  assert(gs.material.transparency == 1);
  assert(gs.material.refractive_index == 1.5);
  Shape a = sphere_glass();
  a.transformation = scaling(2, 2, 2);
  a.material.refractive_index = 1.5;
  Shape b = sphere_glass();
  b.transformation = translation(0, 0, -0.25);
  b.material.refractive_index = 2.0;
  Shape c = sphere_glass();
  c.transformation = translation(0, 0, +0.25);
  c.material.refractive_index = 2.5;
  Ray r = ray(point(0, 0, -4), vector(0, 0, 1));
  Intersections xs = intersections_new(10);
  intersections_insert(&xs, (Intersection){2, a});
  intersections_insert(&xs, (Intersection){2.75, b});
  intersections_insert(&xs, (Intersection){3.25, c});
  intersections_insert(&xs, (Intersection){4.75, b});
  intersections_insert(&xs, (Intersection){5.25, c});
  intersections_insert(&xs, (Intersection){7, a});
  double n1s[6] = {1.0, 1.5, 2.0, 2.5, 2.5, 1.5};
  double n2s[6] = {1.5, 2.0, 2.5, 2.5, 1.5, 1.0};
  for (int i = 0; i < 6; ++i) {
    Computations comp = prepare_computations(xs.hits[i], r, xs);
    assert(comp.n1 == n1s[i]);
    assert(comp.n2 == n2s[i]);
  }
  intersections_free(&xs);
  // .under_point
  r = ray(point(0, 0, -5), vector(0, 0, 1));
  c = sphere_glass();
  c.transformation = translation(0, 0, 1);
  xs = intersections_new(5);
  Intersection i = (Intersection){5, c};
  intersections_insert(&xs, i);
  Computations comp = prepare_computations(i, r, xs);
  assert(near(comp.under_point.z, EPSILON / 2));
  assert(comp.point.z < comp.under_point.z);
  intersections_free(&xs);
  // refracted_color()
  World world = world_default();
  Shape s = world.shapes.shapes[0];
  xs = intersections_new(10);
  intersections_insert(&xs, (Intersection){4, s});
  intersections_insert(&xs, (Intersection){6, s});
  r = ray(point(0, 0, -5), vector(0, 0, 1));
  comp = prepare_computations(xs.hits[0], r, xs);
  assert(color_equal(BLACK, refracted_color(world, comp, 5)));
  intersections_free(&xs);
  world_free(&world);
  // refracted_color() - life=0
  world = world_default();
  s = world.shapes.shapes[0];
  s.material.transparency = 1;
  s.material.refractive_index = 1.5;
  r = ray(point(0, 0, -5), vector(0, 0, 1));
  xs = intersections_new(10);
  intersections_insert(&xs, (Intersection){4, s});
  intersections_insert(&xs, (Intersection){6, s});
  comp = prepare_computations(xs.hits[0], r, xs);
  assert(color_equal(BLACK, refracted_color(world, comp, 0)));
  intersections_free(&xs);
  world_free(&world);
  // refracted_color() - total internal reflection
  world = world_default();
  s = world.shapes.shapes[0];
  s.material.transparency = 1;
  s.material.refractive_index = 1.5;
  r = ray(point(0, 0, sqrt(2) / 2), vector(0, 1, 0));
  xs = intersections_new(10);
  intersections_insert(&xs, (Intersection){-sqrt(2) / 2, s});
  intersections_insert(&xs, (Intersection){+sqrt(2) / 2, s});
  comp = prepare_computations(xs.hits[1], r, xs);
  assert(color_equal(BLACK, refracted_color(world, comp, 5)));
  intersections_free(&xs);
  world_free(&world);
  // refracted_color() - regular
  // NOTE: doesn't work without pointer
  world = world_default();
  world.shapes.shapes[0].material.ambient = 1;
  world.shapes.shapes[0].material.pattern = pattern_test();
  world.shapes.shapes[1].material.transparency = 1;
  world.shapes.shapes[1].material.refractive_index = 1.5;
  xs = intersections_new(10);
  intersections_insert(&xs, (Intersection){-0.9899, world.shapes.shapes[0]});
  intersections_insert(&xs, (Intersection){-0.4899, world.shapes.shapes[1]});
  intersections_insert(&xs, (Intersection){+0.4899, world.shapes.shapes[1]});
  intersections_insert(&xs, (Intersection){+0.9899, world.shapes.shapes[0]});
  r = ray(point(0, 0, 0.1), vector(0, 1, 0));
  comp = prepare_computations(xs.hits[2], r, xs);
  Color rc = refracted_color(world, comp, 5);
  color_print(rc);
  assert(color_equal(color(0, 0.99888, 0.04725), rc));
  intersections_free(&xs);
  world_free(&world);
  // refracted_color() - regular
  world = world_default();
  Shape sfloor = plane();
  sfloor.transformation = translation(0, -1, 0);
  sfloor.material.transparency = 0.5;
  sfloor.material.refractive_index = 1.5;
  world_enter(&world, sfloor);
  Shape ball = sphere();
  ball.material.color = (Color){1, 0, 0};
  ball.material.ambient = 0.5;
  ball.transformation = translation(0, -3.5, -0.5);
  world_enter(&world, ball);
  r = ray(point(0, 0, -3), vector(0, -M_SQRT2 / 2, M_SQRT2 / 2));
  xs = intersections_new(10);
  intersections_insert(&xs, (Intersection){M_SQRT2, sfloor});
  comp = prepare_computations(xs.hits[0], r, xs);
  rc = shade_hit(world, comp, 5);
  assert(color_equal(rc, (Color){0.93642, 0.68642, 0.68642}));
  intersections_free(&xs);
  world_free(&world);
}

void test_intersections(void) {
  Intersections is = intersections_new(10);
  Shape a = sphere();
  Shape b = plane();
  intersections_insert(&is, (Intersection){1, a});
  intersections_insert(&is, (Intersection){2, b});
  intersections_insert(&is, (Intersection){3, a});
  assert(is.count == 3);
  intersections_remove(&is, (Intersection){2, b});
  assert(is.count == 2);
  intersections_remove(&is, (Intersection){88, a});
  assert(is.count == 2);
  assert(intersections_includes(is, (Intersection){3, a}));
  intersections_free(&is);
}

void test_fresnel(void) {
  // schlick() - total internal reflection
  Shape shape = sphere_glass();
  Ray r = ray(point(0, 0, M_SQRT2 / 2), vector(0, 1, 0));
  Intersections xs = intersections_new(10);
  intersections_insert(&xs, (Intersection){-M_SQRT2 / 2, shape});
  intersections_insert(&xs, (Intersection){+M_SQRT2 / 2, shape});
  Computations comp = prepare_computations(xs.hits[1], r, xs);
  assert(near(1, schlick(comp)));
  intersections_free(&xs);
  // schlick() - 90° aka perpendicular
  shape = sphere_glass();
  r = ray(point(0, 0, 0), vector(0, 1, 0));
  xs = intersections_new(10);
  intersections_insert(&xs, (Intersection){-1, shape});
  intersections_insert(&xs, (Intersection){+1, shape});
  comp = prepare_computations(xs.hits[1], r, xs);
  assert(near(0.04, schlick(comp)));
  intersections_free(&xs);
  // schlick() - n2>n1 and small angle
  shape = sphere_glass();
  r = ray(point(0, 0.99, -2), vector(0, 0, 1));
  xs = intersections_new(10);
  intersections_insert(&xs, (Intersection){1.8589, shape});
  comp = prepare_computations(xs.hits[0], r, xs);
  assert(near(0.48874, schlick(comp)));
  intersections_free(&xs);
  // schlick() + shade_hit()
  World w = world_default();
  Shape floor = plane();
  floor.transformation = translation(0, -1, 0);
  floor.material.reflective = 0.5;
  floor.material.transparency = 0.5;
  floor.material.refractive_index = 1.5;
  world_enter(&w, floor);
  Shape ball = sphere();
  ball.material.color = (Color){1, 0, 0};
  ball.material.ambient = 0.5;
  ball.transformation = translation(0, -3.5, -0.5);
  world_enter(&w, ball);
  r = ray(point(0, 0, -3), vector(0, -M_SQRT2 / 2, M_SQRT2 / 2));
  xs = intersections_new(10);
  intersections_insert(&xs, (Intersection){M_SQRT2, floor});
  comp = prepare_computations(xs.hits[0], r, xs);
  assert(color_equal(shade_hit(w, comp, 5), color(0.93391, 0.69643, 0.69243)));
  intersections_free(&xs);
  world_free(&w);
}

void test_cube(void) {
  // Tests: +x -x +y -y +z -z inside
  Shape c = cube();
  Point orig[7] = {
      point(5, 0.5, 0), point(-5, 0.5, 0), point(0.5, 5, 0), point(0.5, -5, 0),
      point(0.5, 0, 5), point(0.5, 0, -5), point(0, 0.5, 0),
  };
  Vector dirs[7] = {
      vector(-1, 0, 0), vector(1, 0, 0), vector(0, -1, 0), vector(0, 1, 0),
      vector(0, 0, -1), vector(0, 0, 1), vector(0, 0, 1),
  };
  double t1s[7] = {4, 4, 4, 4, 4, 4, -1};
  double t2s[7] = {6, 6, 6, 6, 6, 6, +1};
  for (size_t i = 0; i < 7; ++i) {
    Ray r = ray(orig[i], dirs[i]);
    Intersections xs = intersect(c, r);
    assert(xs.count == 2);
    assert(near(xs.hits[0].t, t1s[i]));
    assert(near(xs.hits[1].t, t2s[i]));
    intersections_free(&xs);
  }
  // ray misses cube
  Point m_orig[6] = {
      point(-2, 0, 0), point(0, -2, 0), point(0, 0, -2),
      point(2, 0, 2),  point(0, 2, 2),  point(2, 2, 0),
  };
  Vector m_dirs[6] = {
      vector(0.2673, 0.5345, 0.8018),
      vector(0.8018, 0.2673, 0.5345),
      vector(0.5345, 0.8018, 0.2673),
      vector(0, 0, -1),
      vector(0, -1, 0),
      vector(-1, 0, 0),
  };
  c = cube();
  for (size_t i = 0; i < 6; ++i) {
    Ray r = ray(m_orig[i], m_dirs[i]);
    Intersections xs = intersect(c, r);
    assert(xs.count == 0);
    intersections_free(&xs);
  }
  // cube normal
  Point cpoints[8] = {
      point(1, 0.5, -0.8),  point(-1, -0.2, 0.9), point(-0.4, 1, 0.1),
      point(0.3, -1, -0.7), point(-0.6, 0.3, 1),  point(0.4, 0.4, -1),
      point(1, 1, 1),       point(-1, -1, -1),
  };
  Vector cnormal[8] = {
      vector(1, 0, 0), vector(-1, 0, 0), vector(0, 1, 0), vector(0, -1, 0),
      vector(0, 0, 1), vector(0, 0, -1), vector(1, 0, 0), vector(-1, 0, 0),
  };
  c = cube();
  for (size_t i = 0; i < 8; ++i) {
    Vector n = normal_at(c, cpoints[i]);
    assert(tuple_equal(n, cnormal[i]));
  }
}

void test_csg(void) {
  Shape s1 = cube();
  Shape s2 = sphere();
  Shape *c = csg(CSG_OP_UNION, &s1, &s2);
  assert(c->shape_data.csg.operation == CSG_OP_UNION);
  assert(c->shape_data.csg.left == &s1);
  assert(c->shape_data.csg.right == &s2);
  assert(s1.parent == c);
  assert(s2.parent == c);
  csg_free(c);
  // #5 intersections_filter()
  s1 = sphere();
  s2 = cube();
  Intersections xs = intersections_new(10);
  intersections_insert(&xs, (Intersection){1, s1});
  intersections_insert(&xs, (Intersection){2, s2});
  intersections_insert(&xs, (Intersection){3, s1});
  intersections_insert(&xs, (Intersection){4, s2});
  struct {
    Csg_Op op;
    int idx[2];
  } t[3] = {
      {CSG_OP_UNION, {0, 3}},
      {CSG_OP_INTERSECTION, {1, 2}},
      {CSG_OP_DIFFERENCE, {0, 1}},
  };
  for (size_t i = 0; i < 3; ++i) {
    Shape *c = csg(t[i].op, &s1, &s2);
    Intersections result = intersections_filter(xs, *c);
    assert(result.count == 2);
    assert(intersection_equal(result.hits[0], xs.hits[t[i].idx[0]]));
    assert(intersection_equal(result.hits[1], xs.hits[t[i].idx[1]]));
    csg_free(c);
    intersections_free(&result);
  }
  intersections_free(&xs);
  // #6 - intersect() - miss
  s1 = sphere();
  s2 = cube();
  c = csg(CSG_OP_UNION, &s1, &s2);
  Ray r = ray(point(0, 2, -5), vector(0, 0, 1));
  xs = intersect(*c, r);
  assert(xs.count == 0);
  csg_free(c);
  intersections_free(&xs);
  // #6 - intersect() - hit
  s1 = sphere();
  s2 = sphere();
  s2.transformation = translation(0, 0, 0.5);
  c = csg(CSG_OP_UNION, &s1, &s2);
  r = ray(point(0, 0, -5), vector(0, 0, 1));
  xs = intersect(*c, r);
  assert(xs.count == 2);
  assert(near(xs.hits[0].t, 4));
  assert(near(xs.hits[1].t, 6.5));
  assert(shape_equal(xs.hits[0].object, s1));
  assert(shape_equal(xs.hits[1].object, s2));
  csg_free(c);
  intersections_free(&xs);
}

void test_group(void) {
  Shape g = group();
  assert(m4_equal(g.transformation, M4_IDENTITY));
  assert(g.shape_data.group.childs == NULL);
  group_free(&g);
  Shape s = sphere();
  assert(s.parent == NULL);
  g = group();
  s = sphere();
  group_add(&g, &s);
  assert(s.parent == &g);
  assert(group_includes(g, s));
  group_free(&g);
  // intersection - empty
  g = group();
  Ray r = ray(point(0, 0, 0), vector(0, 0, 1));
  Intersections xs = intersect(g, r);
  assert(xs.count == 0);
  intersections_free(&xs);
  group_free(&g);
  // intersection - 3 spheres, 2 hit, 1 miss
  g = group();
  Shape s1 = sphere(), s2 = sphere(), s3 = sphere();
  s2.transformation = translation(0, 0, -3);
  s3.transformation = translation(5, 0, 0);
  group_add(&g, &s1);
  group_add(&g, &s2);
  group_add(&g, &s3);
  r = ray(point(0, 0, -5), vector(0, 0, 1));
  xs = intersect(g, r);
  assert(xs.count == 4);
  assert(m4_equal(xs.hits[1].object.transformation, s2.transformation));
  assert(shape_equal(xs.hits[0].object, s2));
  assert(shape_equal(xs.hits[1].object, s2));
  assert(shape_equal(xs.hits[2].object, s1));
  assert(shape_equal(xs.hits[3].object, s1));
  intersections_free(&xs);
  group_free(&g);
  // intersect - group+child transform
  g = group();
  g.transformation = scaling(2, 2, 2);
  s = sphere();
  s.transformation = translation(5, 0, 0);
  group_add(&g, &s);
  r = ray(point(10, 0, -10), vector(0, 0, 1));
  xs = intersect(g, r);
  assert(xs.count == 2);
  intersections_free(&xs);
  group_free(&g);
  // normal_at - world_to_object group
  Shape g1 = group();
  Shape g2 = group();
  s = sphere();
  g1.transformation = rotation_y(M_PI_2);
  g2.transformation = scaling(2, 2, 2);
  s.transformation = translation(5, 0, 0);
  group_add(&g1, &g2);
  group_add(&g2, &s);
  Point p = world_to_object(s, point(-2, 0, -10));
  assert(tuple_equal(p, point(0, 0, -1)));
  group_free(&g1);
  group_free(&g2);
  // normal_at - normal_to_world group
  // normal_at - group
  g1 = group();
  g2 = group();
  s = sphere();
  g1.transformation = rotation_y(M_PI_2);
  g2.transformation = scaling(1, 2, 3);
  s.transformation = translation(5, 0, 0);
  group_add(&g1, &g2);
  group_add(&g2, &s);
  Vector n = normal_at(s, point(1.7321, 1.1547, -5.5774));
  assert(tuple_equal(n, vector(0.2857, 0.4286, -0.8571)));
  group_free(&g1);
  group_free(&g2);
}

void test_triangle(void) {
  Point p1 = point(0, 1, 0);
  Point p2 = point(-1, 0, 0);
  Point p3 = point(1, 0, 0);
  Shape t = triangle(p1, p2, p3);
  assert(tuple_equal(t.shape_data.triangle.p1, p1));
  assert(tuple_equal(t.shape_data.triangle.p2, p2));
  assert(tuple_equal(t.shape_data.triangle.p3, p3));
  assert(tuple_equal(t.shape_data.triangle.e1, vector(-1, -1, 0)));
  assert(tuple_equal(t.shape_data.triangle.e2, vector(1, -1, 0)));
  assert(tuple_equal(t.shape_data.triangle.normal, vector(0, 0, -1)));
  // normal is the same
  t = triangle(point(0, 1, 0), point(-1, 0, 0), point(1, 0, 0));
  Vector n1 = normal_at(t, point(0, 0.5, 0));
  Vector n2 = normal_at(t, point(-0.5, 0.75, 0));
  Vector n3 = normal_at(t, point(0.5, 0.25, 0));
  assert(tuple_equal(n1, t.shape_data.triangle.normal));
  assert(tuple_equal(n2, t.shape_data.triangle.normal));
  assert(tuple_equal(n3, t.shape_data.triangle.normal));
  // intersect - misses
  t = triangle(point(0, 1, 0), point(-1, 0, 0), point(1, 0, 0));
  Ray r = ray(point(0, -1, -2), vector(0, 1, 0));
  Intersections xs = intersect(t, r);
  assert(xs.count == 0);
  intersections_free(&xs);
  // intersect - misses p1-p3 edge
  t = triangle(point(0, 1, 0), point(-1, 0, 0), point(1, 0, 0));
  r = ray(point(1, 1, -2), vector(0, 0, 1));
  xs = intersect(t, r);
  assert(xs.count == 0);
  intersections_free(&xs);
  // intersect - misses p1-p2 edge
  t = triangle(point(0, 1, 0), point(-1, 0, 0), point(1, 0, 0));
  r = ray(point(-1, 1, -2), vector(0, 0, 1));
  xs = intersect(t, r);
  assert(xs.count == 0);
  intersections_free(&xs);
  // intersect - misses p2-p3 edge
  t = triangle(point(0, 1, 0), point(-1, 0, 0), point(1, 0, 0));
  r = ray(point(0, -1, -2), vector(0, 0, 1));
  xs = intersect(t, r);
  assert(xs.count == 0);
  intersections_free(&xs);
  // intersect - hits
  t = triangle(point(0, 1, 0), point(-1, 0, 0), point(1, 0, 0));
  r = ray(point(0, 0.5, -2), vector(0, 0, 1));
  xs = intersect(t, r);
  assert(xs.count == 1);
  assert(near(xs.hits[0].t, 2));
  intersections_free(&xs);
}

void test_obj(void) {
  Parser p = obj_parse_file("./test/objs/1gibberish.obj");
  assert(p.n_ignored_lines == 8);
  obj_parser_free(&p);
  p = obj_parse_file("./test/objs/2vertexrecords.obj");
  assert(p.n_ignored_lines == 0);
  obj_parser_free(&p);
  p = obj_parse_file("./test/objs/3trianglefaces.obj");
  assert(p.n_ignored_lines == 1);
  assert(p.default_group.shape_data.group.childs->count == 2);
  Shape t1 = p.default_group.shape_data.group.childs->shapes[0];
  Shape t2 = p.default_group.shape_data.group.childs->shapes[1];
  assert(tuple_equal(t1.shape_data.triangle.p1, p.vertices[1 - 1]));
  assert(tuple_equal(t1.shape_data.triangle.p2, p.vertices[2 - 1]));
  assert(tuple_equal(t1.shape_data.triangle.p3, p.vertices[3 - 1]));
  assert(tuple_equal(t2.shape_data.triangle.p1, p.vertices[1 - 1]));
  assert(tuple_equal(t2.shape_data.triangle.p2, p.vertices[3 - 1]));
  assert(tuple_equal(t2.shape_data.triangle.p3, p.vertices[4 - 1]));
  obj_parser_free(&p);
  {
    Parser p = obj_parse_file("./test/objs/4polygon.obj");
    assert(p.n_ignored_lines == 1);
    assert(p.default_group.shape_data.group.childs->count == 3);
    Shape t1 = p.default_group.shape_data.group.childs->shapes[0];
    Shape t2 = p.default_group.shape_data.group.childs->shapes[1];
    Shape t3 = p.default_group.shape_data.group.childs->shapes[2];
    assert(tuple_equal(t1.shape_data.triangle.p1, p.vertices[1 - 1]));
    assert(tuple_equal(t1.shape_data.triangle.p2, p.vertices[2 - 1]));
    assert(tuple_equal(t1.shape_data.triangle.p3, p.vertices[3 - 1]));
    assert(tuple_equal(t2.shape_data.triangle.p1, p.vertices[1 - 1]));
    assert(tuple_equal(t2.shape_data.triangle.p2, p.vertices[3 - 1]));
    assert(tuple_equal(t2.shape_data.triangle.p3, p.vertices[4 - 1]));
    assert(tuple_equal(t3.shape_data.triangle.p1, p.vertices[1 - 1]));
    assert(tuple_equal(t3.shape_data.triangle.p2, p.vertices[4 - 1]));
    assert(tuple_equal(t3.shape_data.triangle.p3, p.vertices[5 - 1]));
    obj_parser_free(&p);
  }
  // TOdO: named obj groups
}

void test_cylinder(void) {
  { // misses
    struct {
      Point origin;
      Vector direction;
    } t[3] = {
        {point(1, 0, 0), vector(0, 1, 0)},
        {point(0, 0, 0), vector(0, 1, 0)},
        {point(0, 0, -5), vector(1, 1, 1)},
    };
    Shape cyl = cylinder();
    for (size_t i = 0; i < 3; ++i) {
      Vector d = tuple_normalize(t[i].direction);
      Ray r = ray(t[i].origin, d);
      Intersections xs = intersect(cyl, r);
      assert(xs.count == 0);
      intersections_free(&xs);
    }
  }
  { // hits
    struct {
      Point origin;
      Vector direction;
      double t0, t1;
    } t[3] = {
        {point(1, 0, -5), vector(0, 0, 1), 5, 5},
        {point(0, 0, -5), vector(0, 0, 1), 4, 6},
        {point(.5, 0, -5), vector(.1, 1, 1), 6.8079, 7.08872},
    };
    Shape cyl = cylinder();
    for (size_t i = 0; i < ARRAY_LENGTH(t); ++i) {
      Vector direction = tuple_normalize(t[i].direction);
      Ray r = ray(t[i].origin, direction);
      Intersections xs = intersect(cyl, r);
      assert(xs.count == 2);
      assert(near(xs.hits[0].t, t[i].t0));
      assert(near(xs.hits[1].t, t[i].t1));
      intersections_free(&xs);
    }
  }
  { // normal
    struct {
      Point point;
      Vector normal;
    } t[4] = {
        {point(1, 0, 0), vector(1, 0, 0)},
        {point(0, 5, -1), vector(0, 0, -1)},
        {point(0, -2, 1), vector(0, 0, 1)},
        {point(-1, 1, 0), vector(-1, 0, 0)},
    };
    Shape cyl = cylinder();
    for (size_t i = 0; i < ARRAY_LENGTH(t); ++i) {
      Vector n = normal_at(cyl, t[i].point);
      assert(tuple_equal(t[i].normal, n));
    }
  }
  { // limited
    struct {
      Point point;
      Vector direction;
      size_t count;
    } t[6] = {
        {point(0, 1.5, 0), vector(0.1, 1, 0), 0}, // miss
        {point(0, 3, -5), vector(0, 0, 1), 0},    // miss
        {point(0, 0, -5), vector(0, 0, 1), 0},    // miss
        {point(0, 2, -5), vector(0, 0, 1), 0},    // miss
        {point(0, 1, -5), vector(0, 0, 1), 0},    // miss
        {point(0, 1.5, -2), vector(0, 0, 1), 2},  // hit
    };
    Shape cyl = cylinder();
    cyl.shape_data.cylinder.minimum = 1;
    cyl.shape_data.cylinder.maximum = 2;
    for (size_t i = 0; i < ARRAY_LENGTH(t); ++i) {
      Vector direction = tuple_normalize(t[i].direction);
      Ray r = ray(t[i].point, direction);
      Intersections xs = intersect(cyl, r);
      assert(xs.count == t[i].count);
      intersections_free(&xs);
    }
  }
  { // closed cap intersections
    struct {
      Point point;
      Vector direction;
      size_t count;
    } t[5] = {
        {point(0, 3, 0), vector(0, -1, 0), 2},
        {point(0, 3, -2), vector(0, -1, 2), 2},
        {point(0, 4, -2), vector(0, -1, 1), 2}, // corner case
        {point(0, 0, -2), vector(0, 1, 2), 2},
        {point(0, -1, -2), vector(0, 1, 1), 2}, // corner case
    };
    Shape cyl = cylinder();
    cyl.shape_data.cylinder.minimum = 1;
    cyl.shape_data.cylinder.maximum = 2;
    cyl.shape_data.cylinder.closed = true;
    for (size_t i = 0; i < ARRAY_LENGTH(t); ++i) {
      Vector direction = tuple_normalize(t[i].direction);
      Ray r = ray(t[i].point, direction);
      Intersections xs = intersect(cyl, r);
      assert(xs.count == t[i].count);
      intersections_free(&xs);
    }
  }
}

void test_area_shadow(void) {
  { // is_shadowed() - already worked
    struct {
      Point point;
      bool result;
    } t[4] = {
        {point(-10, -10, 10), false},
        {point(10, 10, 10), true},
        {point(-20, -20, -20), false},
        {point(-5, -5, -5), false},
    };
    World w = world_default();
    Point lpos = point(-10, -10, -10);
    for (size_t i = 0; i < ARRAY_LENGTH(t); ++i) {
      assert(is_shadowed(w, t[i].point, lpos) == t[i].result);
    }
    world_free(&w);
  }
  { // intensity_at()
    struct {
      Point point;
      double result;
    } t[7] = {
        {point(0, 1.0001, 0), 1.0},  {point(-1.0001, 0, 0), 1.0},
        {point(0, 0, -1.0001), 1.0}, {point(0, 0, 1.0001), 0.0},
        {point(1.0001, 0, 0), 0.0},  {point(0, -1.0001, 0), 0.0},
        {point(0, 0, 0), 0.0},
    };
    World w = world_default();
    Light light = w.lights[0];
    for (size_t i = 0; i < ARRAY_LENGTH(t); ++i) {
      Point pt = t[i].point;
      assert(near(t[i].result, intensity_at(light, pt, w)));
    }
    world_free(&w);
  }
  { // lighting() using intensity_at()
    struct {
      double intensity;
      Color result;
    } t[3] = {
        {1.0, {1, 1, 1}},
        {0.5, {0.55, 0.55, 0.55}},
        {0.0, {0.1, 0.1, 0.1}},
    };
    World w = world_default();
    w.lights[0].position = point(0, 0, -10);
    w.shapes.shapes[0].material.ambient = 0.1;
    w.shapes.shapes[0].material.diffuse = 0.9;
    w.shapes.shapes[0].material.specular = 0;
    w.shapes.shapes[0].material.color = WHITE;
    Point pt = point(0, 0, 1);
    Vector eye = vector(0, 0, -1);
    Vector normal = vector(0, 0, -1);
    for (size_t i = 0; i < ARRAY_LENGTH(t); ++i) {
      Color result = lighting(w.shapes.shapes[0].material, w.shapes.shapes[0],
                              pt, w.lights[0], eye, normal, t[i].intensity);
      assert(color_equal(result, t[i].result));
    }
    world_free(&w);
  }
  { // creating arealight()
    Point corner = point(0, 0, 0);
    Vector v1 = vector(2, 0, 0);
    Vector v2 = vector(0, 0, 1);
    Light light = arealight(corner, v1, 4, v2, 2, WHITE);
    assert(tuple_equal(light.light_data.area.corner, corner));
    assert(tuple_equal(light.light_data.area.uvec, vector(0.5, 0, 0)));
    assert(tuple_equal(light.light_data.area.vvec, vector(0, 0, 0.5)));
    assert(light.light_data.area.usteps == 4);
    assert(light.light_data.area.vsteps == 2);
    assert(light.light_data.area.samples == 8);
    assert(tuple_equal(light.position, point(1, 0, 0.5)));
  }
  { // point_on_light()
    struct {
      uint8_t u, v;
      Point result;
    } t[5] = {
        {0, 0, point(0.25, 0, 0.25)}, {1, 0, point(0.75, 0, 0.25)},
        {0, 1, point(0.25, 0, 0.75)}, {2, 0, point(1.25, 0, 0.25)},
        {3, 1, point(1.75, 0, 0.75)},
    };
    Point corner = point(0, 0, 0);
    Vector v1 = vector(2, 0, 0);
    Vector v2 = vector(0, 0, 1);
    Light light = arealight(corner, v1, 4, v2, 2, WHITE);
    for (size_t i = 0; i < ARRAY_LENGTH(t); ++i) {
      assert(tuple_equal(t[i].result, point_on_light(light, t[i].u, t[i].v)));
    }
  }
}
void test_cone(void) {}

int main(void) {
  test_tuple();
  test_canvas();
  test_matrix();
  test_transformation();
  test_raycasting();
  test_shading();
  test_world();
  test_shadow();
  test_plane();
  test_patterns();
  test_reflections();
  test_intersections();
  test_refraction();
  test_fresnel();
  test_cube();
  test_csg();
  test_group();
  test_triangle();
  test_obj();
  test_cylinder();
  test_cone();
  test_area_shadow();
  printf("ALL OK!\n");
  return 0;
}
