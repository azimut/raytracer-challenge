#include "./world.h"
#include "./transformation.h"
#include <stdio.h>

void world_enter(World *world, Shape s) {
  if (world->objects) {
    world->objects =
        reallocarray(world->objects, world->count + 1, sizeof(Shape));
  } else {
    world->objects = calloc(1, sizeof(Shape));
  }
  if (world->objects == NULL) {
    perror("alloc in world_enter()");
  }
  world->objects[world->count] = s;
  world->count++;
}

World world_default(void) {
  World w = {0};
  w.light = pointlight(point(-10, 10, -10), color(1, 1, 1));

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
  free(world->objects);
  world->objects = NULL;
}

Intersections world_intersect(World world, Ray ray) {
  Intersections is = {0};
  for (size_t i = 0; i < world.count; ++i) {
    /* fprintf(stderr, "Intersecting: ID=%d\n", world.objects[i].id); */
    Intersections temp_is = intersect(world.objects[i], ray);
    if (temp_is.count == 0) {
      /* fprintf(stderr, "Skipping %d\n", world.objects[i].id); */
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
    free_intersections(&temp_is);
  }
  intersections_sort(&is);
  return is;
}

Color shade_hit(World world, Computations comp, uint8_t life) {
  bool shadowed = is_shadowed(world, comp.over_point);
  Color surface = lighting(comp.object.material, comp.object, comp.over_point,
                           world.light, comp.eye, comp.normal, shadowed);
  Color reflected = reflected_color(world, comp, life);
  return color_add(surface, reflected);
}

Color color_at(World world, Ray ray, uint8_t life) {
  Intersections is = world_intersect(world, ray);
  Intersection *i = hit(is);
  Color color = (Color){0, 0, 0};
  if (i) {
    Computations comp = prepare_computations(*i, ray);
    color = shade_hit(world, comp, life);
  }
  free_intersections(&is);
  return color;
}

bool is_shadowed(World w, Point p) {
  Vector shadow_vector = tuple_sub(w.light.position, p);
  double distance = tuple_length(shadow_vector);
  Ray shadow_ray = ray(p, tuple_normalize(shadow_vector));
  Intersections is = world_intersect(w, shadow_ray);
  Intersection *h = hit(is);
  bool shadowed = false;
  if (h && h->t < distance) {
    shadowed = true;
  }
  free_intersections(&is);
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
