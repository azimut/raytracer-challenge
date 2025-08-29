#include "./shading.h"
#include <assert.h>
#include <math.h>

Vector reflect(Vector v, Vector n) {
  assert(is_vector(v) && is_vector(n));
  return tuple_sub(v, tuple_smul(tuple_smul(n, 2.0), tuple_dot_product(v, n)));
}

PointLight pointlight(Point position, Color intensity) {
  assert(is_point(position));
  return (PointLight){position, intensity};
}

Color lighting(MaterialPhong material, Shape object, Point point,
               PointLight light, Vector eye, Vector normal, bool in_shadow) {
  assert(is_point(point) && is_vector(eye) && is_vector(normal));
  Color material_color =
      material.pattern.ptype == PATTERN_TYPE_NONE
          ? material.color
          : pattern_at_shape(material.pattern, object, point);
  Color effective_color = color_mul(material_color, light.intensity);
  Color ambient = color_smul(effective_color, material.ambient);
  Vector dir_light = tuple_normalize(tuple_sub(light.position, point));
  double light_dot_normal = tuple_dot_product(dir_light, normal);
  Color diffuse = {0, 0, 0}, specular = {0, 0, 0};
  double attenuation = 1;
  if (!in_shadow && light_dot_normal >= 0) {
    /* double distance = tuple_length(tuple_sub(light.position, point)); */
    /* attenuation = 1 / (1.0 + 0.14 * distance + 0.07 * distance); */
    diffuse = color_smul(effective_color, material.diffuse * light_dot_normal);
    Vector reflection = reflect(tuple_neg(dir_light), normal);
    double reflect_dot_eye = tuple_dot_product(reflection, eye);
    if (reflect_dot_eye > 0) {
      double factor = pow(reflect_dot_eye, material.shininess);
      specular = color_smul(light.intensity, material.specular * factor);
    }
  }
  return color_add(color_smul(color_add(specular, diffuse), attenuation),
                   ambient);
}
