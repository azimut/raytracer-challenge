#include "./world.h"
#include "./transformation.h"
#include <assert.h>
#include <stdio.h>

void world_enter(World *world, Shape s) {
  if (world->shapes) {
    world->shapes =
        reallocarray(world->shapes, world->shapes_count + 1, sizeof(Shape));
  } else {
    world->shapes = calloc(1, sizeof(Shape));
  }
  if (world->shapes == NULL) {
    perror("alloc in world_enter()");
  }
  world->shapes[world->shapes_count] = s;
  world->shapes_count++;
}

void world_enlight(World *world, PointLight light) {
  if (world->lights) {
    world->lights = reallocarray(world->lights, world->lights_count + 1,
                                 sizeof(PointLight));
  } else {
    world->lights = calloc(1, sizeof(PointLight));
  }
  if (world->lights == NULL)
    perror("alloc in world_enlight()");
  world->lights[world->lights_count] = light;
  world->lights_count++;
}

World world_default(void) {
  World w = {0};

  PointLight pl = pointlight(point(-10, 10, -10), WHITE);
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
  free(world->shapes);
  world->shapes = NULL;
  free(world->lights);
  world->lights = NULL;
}

Intersections world_intersect(World world, Ray ray) {
  Intersections is = intersections_new(10);
  for (size_t i = 0; i < world.shapes_count; ++i) {
    Intersections temp_is = intersect(world.shapes[i], ray);
    if (temp_is.count == 0) {
      intersections_free(&temp_is);
      continue;
    }

    if (is.hits) {
      is.hits =
          reallocarray(is.hits, is.count + temp_is.count, sizeof(Intersection));
    } else {
      is.hits = calloc(temp_is.count, sizeof(Intersection));
    }
    if (is.hits == NULL) {
      perror("alloc?");
    }
    for (size_t j = 0; j < temp_is.count; ++j) {
      is.hits[is.count + j] = temp_is.hits[j];
    }

    is.count += temp_is.count;
    intersections_free(&temp_is);
  }
  intersections_sort(&is);
  return is;
}

Color shade_hit(World world, Computations comp, uint8_t life) {
  Color surface = BLACK;
  for (size_t i = 0; i < world.lights_count; ++i) {
    bool shadowed =
        is_shadowed(world, comp.over_point, world.lights[i].position);
    Color new = lighting(comp.object.material, comp.object, comp.over_point,
                         world.lights[i], comp.eye, comp.normal, shadowed);
    surface = color_add(surface, new);
  }
  Color reflected = reflected_color(world, comp, life);
  return color_add(surface, reflected);
}

Color color_at(World world, Ray ray, uint8_t life) {
  Intersections is = world_intersect(world, ray);
  Intersection *i = hit(is);
  Color color = (Color){0, 0, 0};
  if (i) {
    Computations comp = prepare_computations(*i, ray, is);
    color = shade_hit(world, comp, life);
  }
  intersections_free(&is);
  return color;
}

bool is_shadowed(World w, Point p, Point light_pos) {
  assert(is_point(p) && is_point(light_pos));
  Vector shadow_vector = tuple_sub(light_pos, p);
  double distance = tuple_length(shadow_vector);
  Ray shadow_ray = ray(p, tuple_normalize(shadow_vector));
  Intersections is = world_intersect(w, shadow_ray);
  Intersection *h = hit(is);
  bool shadowed = false;
  if (h && h->t < distance) {
    shadowed = true;
  }
  intersections_free(&is);
  return shadowed;
}

Color reflected_color(World world, Computations comp, uint8_t life) {
  if (comp.object.material.reflective == 0 || life == 0) {
    return BLACK;
  }
  Ray r = ray(comp.over_point, comp.reflect); // avoid self-reflection
  Color color = color_at(world, r, life - 1);
  return color_smul(color, comp.object.material.reflective);
}
