#include "./shading.h"
#include <assert.h>
#include <math.h>

Vector normal_at(Sphere s, Point world_point) {
  assert(is_point(world_point));
  Point object_point = m4_tmul(m4_inverse(s.transformation), world_point);
  Vector object_normal = tuple_sub(object_point, point(0, 0, 0));
  Vector world_normal =
      m4_tmul(m4_transpose(m4_inverse(s.transformation)), object_normal);
  world_normal.w = 0;
  return tuple_normalize(world_normal);
}

Vector reflect(Vector v, Vector n) {
  assert(is_vector(v) && is_vector(n));
  return tuple_sub(v, tuple_smul(tuple_smul(n, 2.0f), tuple_dot_product(v, n)));
}

PointLight pointlight(Point position, Color intensity) {
  assert(is_point(position));
  return (PointLight){position, intensity};
}

Color lighting(MaterialPhong material, Point point, PointLight light,
               Vector eye, Vector normal, bool in_shadow) {
  assert(is_point(point) && is_vector(eye) && is_vector(normal));
  Color effective_color = color_mul(material.color, light.intensity);
  Color ambient = color_smul(effective_color, material.ambient);
  Vector dir_light = tuple_normalize(tuple_sub(light.position, point));
  float light_dot_normal = tuple_dot_product(dir_light, normal);
  Color diffuse = {0, 0, 0}, specular = {0, 0, 0};
  if (!in_shadow && light_dot_normal >= 0) {
    diffuse = color_smul(effective_color, material.diffuse * light_dot_normal);
    Vector reflection = reflect(tuple_neg(dir_light), normal);
    float reflect_dot_eye = tuple_dot_product(reflection, eye);
    if (reflect_dot_eye > 0) {
      float factor = powf(reflect_dot_eye, material.shininess);
      specular = color_smul(light.intensity, material.specular * factor);
    }
  }
  return color_add(color_add(ambient, diffuse), specular);
}
