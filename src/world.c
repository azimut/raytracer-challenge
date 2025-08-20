#include "./world.h"
#include "./transformation.h"
#include <stdio.h>

void world_enter(World *world, Sphere s) {
  if (world->objects) {
    world->objects =
        reallocarray(world->objects, world->count + 1, sizeof(Sphere));
  } else {
    world->objects = calloc(1, sizeof(Sphere));
  }
  world->objects[world->count] = s;
  world->count++;
}

World world_default(void) {
  World w = {0};
  w.light = pointlight(point(-10, 10, -10), color(1, 1, 1));

  Sphere s1 = sphere();
  MaterialPhong m1 = material();
  m1.color = color(0.8, 1.0, 0.6);
  m1.diffuse = 0.7;
  m1.specular = 0.2;
  set_material(&s1, m1);
  world_enter(&w, s1);

  Sphere s2 = sphere();
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
    fprintf(stderr, "Intersecting: ID=%d\n", world.objects[i].id);
    Intersections temp_is = intersect(world.objects[i], ray);
    if (temp_is.count == 0) {
      fprintf(stderr, "Skipping %d\n", world.objects[i].id);
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
    free_intersections(temp_is);
  }
  intersections_sort(&is);
  return is;
}
