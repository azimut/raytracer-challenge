#ifndef OBJ_H
#define OBJ_H

#include "./shapes.h"
#include "./tuple.h"
#include <stdlib.h>

typedef struct Parser {
  size_t n_ignored_lines;
  size_t capacity;
  size_t count;
  Shape default_group;
  Point *vertices;
} Parser;

Parser obj_parse_file(const char *filename);
void obj_parser_free(Parser *);

#endif /* OBJ_H */
