#include "./canvas.h"
#include "./util.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Canvas canvas(size_t w, size_t h) {
  Color *grid = calloc(w * h, sizeof(Color));
  return (Canvas){.grid = grid, .width = w, .height = h};
}

Color canvas_get(Canvas canvas, size_t row, size_t col) {
  return canvas.grid[col + (row * canvas.width)];
}

void canvas_set(Canvas *canvas, size_t row, size_t col, Color value) {
  canvas->grid[col + (row * canvas->width)] = value;
}

static unsigned char pixel(double channel) {
  const double gamma = 1.0;
  double value = pow(1.0 - exp(-channel * gamma), 1.0 / 2.2);
  /* double value = clamp(channel, 0.0, 1.0); */
  return floor(value * 255);
}

static void canvas_stream(Canvas canvas, FILE *fp) {
  fprintf(fp, "P3\n");
  fprintf(fp, "%lu %lu\n", canvas.width, canvas.height);
  fprintf(fp, "255\n");
  const size_t max_width = 5; // 72 = 4*3*6 // 4chars 3channels 6pixels
  size_t width = 0;
  for (size_t row = 0; row < canvas.height; ++row) {
    for (size_t col = 0; col < canvas.width; ++col) {
      Color current = canvas_get(canvas, row, col);
      unsigned char r = pixel(current.red);
      unsigned char g = pixel(current.green);
      unsigned char b = pixel(current.blue);
      fprintf(fp, "%3d %3d %3d", r, g, b);
      if (++width > max_width) {
        fprintf(fp, "\n");
        width = 0;
      } else {
        fprintf(fp, " ");
      }
    }
  }
}

void canvas_print(Canvas canvas) { canvas_stream(canvas, stdout); }

void canvas_save_frame(Canvas canvas, char *filename, uint16_t nframe,
                       Point pos) {
  char *buf = calloc(200, sizeof(char));
  sprintf(buf, "media/%s_%04d_%.2f_%.2f_%.2f.ppm", filename, nframe, pos.x,
          pos.y, pos.z);
  canvas_save(canvas, buf);
  free(buf);
}

void canvas_save(Canvas canvas, char *filepath) {
  FILE *fp = fopen(filepath, "w");
  canvas_stream(canvas, fp);
  fprintf(stderr, "Created: %s\n", filepath);
  fclose(fp);
}

void canvas_free(Canvas *canvas) {
  free(canvas->grid);
  canvas->grid = NULL;
}
