#include "./world.h"
#include "./transformation.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

void world_enter(World *world, Shape s) { shapes_append(&world->shapes, s); }

void world_enlight(World *world, Light light) {
  if (world->lights) {
    world->lights =
        reallocarray(world->lights, world->lights_count + 1, sizeof(Light));
  } else {
    world->lights = calloc(1, sizeof(Light));
  }
  if (world->lights == NULL) {
    perror("alloc in world_enlight()");
    exit(EXIT_FAILURE);
  }
  world->lights[world->lights_count] = light;
  world->lights_count++;
}

World world_default(void) {
  World w = {0};

  Light pl = pointlight(point(-10, 10, -10), WHITE);
  world_enlight(&w, pl);

  Shape s1 = sphere();
  s1.material.color = color(0.8, 1.0, 0.6);
  s1.material.diffuse = 0.7;
  s1.material.specular = 0.2;
  world_enter(&w, s1);

  Shape s2 = sphere();
  set_transform(&s2, scaling(0.5, 0.5, 0.5));
  world_enter(&w, s2);

  return w;
}

void world_free(World *world) {
  shapes_free(&world->shapes);
  free(world->lights);
  world->lights = NULL;
}

Intersections world_intersect(const World world, const Ray ray) {
  Intersections is = intersections_new(15);
  for (size_t i = 0; i < world.shapes.count; ++i) {
    Intersections temp_is = intersect(world.shapes.shapes[i], ray);
    if (temp_is.count == 0) {
      intersections_free(&temp_is);
      continue;
    }
    for (size_t j = 0; j < temp_is.count; ++j) {
      intersections_insert(&is, temp_is.hits[j]);
    }
    intersections_free(&temp_is);
  }
  intersections_sort(&is);
  return is;
}

Color shade_hit(const World world, const Computations comp, uint8_t life) {
  Color surface = {0};
  for (size_t i = 0; i < world.lights_count; ++i) {
    const Vector light_pos = world.lights[i].position;
    const bool shadowed = is_shadowed(world, comp.over_point, light_pos);
    const Color new =
        lighting(comp.object.material, comp.object, comp.over_point,
                 world.lights[i], comp.eye, comp.normal, shadowed);
    surface = color_add(surface, new);
  }
  const Color reflected = reflected_color(world, comp, life);
  const Color refraction = refracted_color(world, comp, life);
  const MaterialPhong m = comp.object.material;
  if (m.reflective > 0 && m.transparency > 0) {
    const double reflectance = schlick(comp);
    surface = color_add(surface,
                        color_add(color_smul(reflected, reflectance),
                                  color_smul(refraction, (1.0 - reflectance))));
  } else {
    surface = color_add(color_add(surface, reflected), refraction);
  }
  return surface;
}

Color color_at(const World world, const Ray ray, uint8_t life) {
  Intersections is = world_intersect(world, ray);
  Intersection *i = hit(is);
  Color pixel = BLACK;
  if (i) {
    const Computations comp = prepare_computations(*i, ray, is);
    pixel = shade_hit(world, comp, life);
  }
#ifdef GRADIENT
  else {
    const Color a = color(1.0, 0.1, 0.9);
    const Color b = color(0.1, 0.9, 1.0);
    const double y = tuple_normalize(ray.direction).y;
    const double t = 0.5 * (y + 1.0);
    pixel = color_add(color_smul(a, (1.0 - t)), color_smul(b, t));
  }
#endif
  intersections_free(&is);
  return pixel;
}

bool is_shadowed(const World world, const Point p, const Point light_pos) {
#ifndef BLAZE
  assert(is_point(p) && is_point(light_pos));
#endif
  const Vector shadow_vector = tuple_sub(light_pos, p);
  const Ray shadow_ray = ray(p, tuple_normalize(shadow_vector));
  Intersections is = world_intersect(world, shadow_ray);
  Intersection *h = hit(is);
  const double distance = tuple_length(shadow_vector);
  bool shadowed = false;
  if (h && h->t < distance) {
    shadowed = true;
  }
  intersections_free(&is);
  return shadowed;
}

Color reflected_color(const World world, const Computations comp,
                      uint8_t life) {
  if (!life || !comp.object.material.reflective) {
    return BLACK;
  }
  const Ray r = ray(comp.over_point, comp.reflect); // avoid self-reflection
  const Color color = color_at(world, r, life - 1);
  return color_smul(color, comp.object.material.reflective);
}

Color refracted_color(const World world, const Computations comp,
                      uint8_t life) {
  const bool total_ireflection = comp.sin2_t > 1;
  if (!life || !comp.object.material.transparency || total_ireflection) {
    return BLACK;
  }
  const Vector direction = tuple_sub(
      tuple_smul(comp.normal, ((comp.n_ratio * comp.cos_i) - comp.cos_t)),
      tuple_smul(comp.eye, comp.n_ratio));
  const Ray r = ray(comp.under_point, direction);
  return color_smul(color_at(world, r, life - 1),
                    comp.object.material.transparency);
}

double intensity_at(const Light light, const Point point, const World world) {
  double result = 1;
  switch (light.ltype) {
  case LIGHT_TYPE_POINT: {
    result = is_shadowed(world, point, light.position) ? 0 : 1;
    break;
  }
  case LIGHT_TYPE_AREA: {
    fprintf(stderr, "Not implemented on area lights yet.\n");
    exit(EXIT_FAILURE);
    break;
  }
  }
  return result;
};
