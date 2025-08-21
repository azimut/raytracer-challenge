#include "./canvas.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Canvas canvas(size_t width, size_t height) {
  Color *grid = calloc(width * height, sizeof(Color));
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
  const size_t max_width = 5; // 72 = 4*3*6 // 4chars 3channels 6pixels
  size_t width = 0;
  for (size_t row = 0; row < canvas.height; ++row) {
    for (size_t col = 0; col < canvas.width; ++col) {
      Color current = canvas_get(canvas, row, col);
      unsigned char r = pixel(current.red);
      unsigned char g = pixel(current.green);
      unsigned char b = pixel(current.blue);
      printf("%3d %3d %3d", r, g, b);
      if (++width > max_width) {
        printf("\n");
        width = 0;
      } else {
        printf(" ");
      }
    }
  }
}

void canvas_free(Canvas *canvas) {
  free(canvas->grid);
  canvas->grid = NULL;
}
