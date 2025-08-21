#ifndef CANVAS_H
#define CANVAS_H

#include "./color.h"
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

#endif /* CANVAS_H */
