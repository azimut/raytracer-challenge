#include "./shading.h"
#include <assert.h>
#include <math.h>

Vector reflect(const Vector v, const Vector n) {
#ifndef BLAZE
  assert(is_vector(v) && is_vector(n));
#endif
  return tuple_sub(v, tuple_smul(tuple_smul(n, 2.0), tuple_dot_product(v, n)));
}

PointLight pointlight(const Point position, const Color intensity) {
#ifndef BLAZE
  assert(is_point(position));
#endif
  return (PointLight){.position = position, .intensity = intensity};
}

Color lighting(const MaterialPhong material, const Shape object,
               const Point point, const PointLight light, const Vector eye,
               const Vector normal, bool in_shadow) {
#ifndef BLAZE
  assert(is_point(point) && is_vector(eye) && is_vector(normal));
#endif
  const Color material_color =
      material.pattern.ptype == PATTERN_TYPE_NONE
          ? material.color
          : pattern_at_shape(material.pattern, object, point);
  const Color effective_color = color_mul(material_color, light.intensity);
  const Color ambient = color_smul(effective_color, material.ambient);
  const Vector dir_light = tuple_normalize(tuple_sub(light.position, point));
  const double light_dot_normal = tuple_dot_product(dir_light, normal);
  Color diffuse = BLACK, specular = BLACK;
  double attenuation = 1;
  if (!in_shadow && light_dot_normal >= 0) {
    const double distance = tuple_length(tuple_sub(light.position, point));
    const double linear = light.attenuation.linear;
    const double quadratic = light.attenuation.quadratic;
    attenuation = 1 / (1.0 + linear * distance + quadratic * distance);
    diffuse = color_smul(effective_color, material.diffuse * light_dot_normal);
    const Vector reflection = reflect(tuple_neg(dir_light), normal);
    const double reflect_dot_eye = tuple_dot_product(reflection, eye);
    if (reflect_dot_eye > 0) {
      const double factor = pow(reflect_dot_eye, material.shininess);
      specular = color_smul(light.intensity, material.specular * factor);
    }
  }
  return color_smul(color_add(color_add(specular, diffuse), ambient),
                    attenuation);
}
