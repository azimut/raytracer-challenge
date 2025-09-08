#include "./shading.h"
#include "./world.h"
#include <assert.h>
#include <math.h>

Vector reflect(const Vector v, const Vector n) {
#ifndef BLAZE
  assert(is_vector(v) && is_vector(n));
#endif
  return tuple_sub(v, tuple_smul(tuple_smul(n, 2.0), tuple_dot_product(v, n)));
}

Light pointlight(const Point position, const Color intensity) {
#ifndef BLAZE
  assert(is_point(position));
#endif
  return (Light){
      .position = position, .intensity = intensity, .ltype = LIGHT_TYPE_POINT};
}

Light arealight(const Point corner, const Vector v1, const uint8_t usteps,
                const Vector v2, const uint8_t vsteps, const Color intensity) {
  const Point position =
      tuple_add(corner, tuple_add(tuple_sdiv(v1, 2), tuple_sdiv(v2, 2)));
  return (Light){
      .ltype = LIGHT_TYPE_AREA,
      .position = position,
      .intensity = intensity,
      .light_data.area.corner = corner,
      .light_data.area.usteps = usteps,
      .light_data.area.vsteps = vsteps,
      .light_data.area.samples = usteps * vsteps,
      .light_data.area.uvec = tuple_sdiv(v1, usteps),
      .light_data.area.vvec = tuple_sdiv(v2, vsteps),
  };
}

Color lighting(const MaterialPhong material, const Shape object,
               const Point point, const Light light, const Vector eye,
               const Vector normal, double intensity) {
#ifndef BLAZE
  assert(is_point(point) && is_vector(eye) && is_vector(normal));
#endif
  const Color material_color =
      material.pattern.ptype == PATTERN_TYPE_NONE
          ? material.color
          : pattern_at_shape(material.pattern, object, point);
  const Color effective_color = color_mul(material_color, light.intensity);
  const Color ambient = color_smul(effective_color, material.ambient);
  Color diffuse = BLACK, specular = BLACK;
  const double distance = tuple_length(tuple_sub(light.position, point));
  const double linear = light.attenuation.linear;
  const double quadratic = light.attenuation.quadratic;
  double attenuation = 1 / (1.0 + linear * distance + quadratic * distance);
  switch (light.ltype) {
  case LIGHT_TYPE_POINT: {
    const Vector dir_light = tuple_normalize(tuple_sub(light.position, point));
    const double light_dot_normal = tuple_dot_product(dir_light, normal);
    if (light_dot_normal >= 0) {
      diffuse = color_smul(effective_color,
                           material.diffuse * light_dot_normal * intensity);
      const Vector reflection = reflect(tuple_neg(dir_light), normal);
      const double reflect_dot_eye = tuple_dot_product(reflection, eye);
      if (reflect_dot_eye > 0) {
        const double factor = pow(reflect_dot_eye, material.shininess);
        specular =
            color_smul(light.intensity, material.specular * factor * intensity);
      }
    }
    break;
  }
  case LIGHT_TYPE_AREA: {
    for (size_t u = 0; u < light.light_data.area.usteps; ++u) {
      for (size_t v = 0; v < light.light_data.area.vsteps; ++v) {
        const Vector dir_light =
            tuple_normalize(tuple_sub(point_on_light(light, u, v), point));
        const double light_dot_normal = tuple_dot_product(dir_light, normal);
        if (light_dot_normal >= 0) {
          diffuse =
              color_add(diffuse, color_smul(effective_color,
                                            material.diffuse *
                                                light_dot_normal * intensity));
          const Vector reflection = reflect(tuple_neg(dir_light), normal);
          const double reflect_dot_eye = tuple_dot_product(reflection, eye);
          if (reflect_dot_eye > 0) {
            const double factor = pow(reflect_dot_eye, material.shininess);
            specular = color_add(
                specular, color_smul(light.intensity,
                                     material.specular * factor * intensity));
          }
        }
      }
    }
    diffuse = color_sdiv(diffuse, light.light_data.area.samples);
    specular = color_sdiv(specular, light.light_data.area.samples);
    break;
  }
  }
  return color_smul(color_add(color_add(specular, diffuse), ambient),
                    attenuation);
}
