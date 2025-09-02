#ifndef CANVAS_H
#define CANVAS_H

#include "./color.h"
#include "./tuple.h"
#include <stdint.h>
#include <stdlib.h>

typedef struct Canvas {
  Color *grid;
  size_t width, height;
} Canvas;

Canvas canvas(size_t, size_t);
Color canvas_get(Canvas, size_t, size_t);
void canvas_set(Canvas *, size_t, size_t, Color);
void canvas_print(Canvas);
void canvas_free(Canvas *);
void canvas_save(Canvas, char *);
void canvas_save_frame(Canvas, char *, uint16_t, Point);

#endif /* CANVAS_H */
