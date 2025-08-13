#define MAX_LINE_WIDTH 70
#define MAX_COMPONENTS MAX_LINE_WIDTH / 4

#include "./color.h"
#include <stdlib.h>

typedef struct Canvas {
  Color *grid;
  size_t width, height;
} Canvas;

Canvas canvas(size_t, size_t);
Color canvas_get(Canvas, int, int);
void canvas_set(Canvas *, int, int, Color);
void canvas_print(Canvas);
void canvas_free(Canvas *);
