#include "./canvas.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Canvas canvas(size_t width, size_t height) {
  Color *grid = malloc(width * height * sizeof(Color));
  memset(grid, 0, width * height * sizeof(Color));
  return (Canvas){grid, width, height};
}

Color canvas_get(Canvas canvas, int row, int col) {
  return canvas.grid[col + row * canvas.height];
}

void canvas_set(Canvas *canvas, int row, int col, Color value) {
  canvas->grid[col + row * canvas->height] = value;
}

static unsigned char pixel(float channel) {
  return fmin(fmax(channel, 0.0), 1.0) * 255;
}

void canvas_print(Canvas canvas) {
  printf("P3\n");
  printf("%lu %lu\n", canvas.height, canvas.width);
  printf("255\n");
  for (size_t row = 0; row < canvas.width; ++row) {
    for (size_t col = 0; col < canvas.height; ++col) {
      Color current = canvas_get(canvas, row, col);
      printf("%d %d %d\n", pixel(current.red), pixel(current.green),
             pixel(current.blue));
    }
  }
}

void canvas_free(Canvas *canvas) { free(canvas->grid); }
