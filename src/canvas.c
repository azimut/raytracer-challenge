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
  return fmin(fmax(channel, 0.0f), 1.0f) * 255;
}

void canvas_print(Canvas canvas) {
  printf("P3\n");
  printf("%lu %lu\n", canvas.height, canvas.width);
  printf("255\n");
  for (size_t row = 0; row < canvas.height; ++row) {
    for (size_t col = 0; col < canvas.width; ++col) {
      Color current = canvas_get(canvas, row, col);
      unsigned char r = pixel(current.red);
      unsigned char g = pixel(current.green);
      unsigned char b = pixel(current.blue);
      printf("%d %d %d\n", r, g, b);
    }
  }
}

void canvas_free(Canvas *canvas) { free(canvas->grid); }
