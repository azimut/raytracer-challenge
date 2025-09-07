#include "./obj.h"
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

static const int buffsize = 200;
static const int init_capacity = 10;
static const int renew_capacity = 10;

static Parser parser_new(void) {
  return (Parser){
      .default_group = group(),
      .capacity = init_capacity,
      .vertices = calloc(init_capacity, sizeof(Point)),
  };
}

static void parser_insert_vertex(Parser *parser, const Point vertex) {
  if (parser->count + 1 > parser->capacity) {
    parser->capacity += renew_capacity;
    parser->vertices =
        reallocarray(parser->vertices, parser->capacity, sizeof(Point));
  }
  parser->vertices[parser->count] = vertex;
  parser->count++;
}

typedef struct Indices {
  long *idxs;
  size_t count;
  size_t capacity;
} Indices;

static Indices indices_new() {
  return (Indices){
      .idxs = calloc(5, sizeof(long)),
      .capacity = 5,
  };
}

static void indices_free(Indices *is) {
  if (is->idxs) {
    free(is->idxs);
    is->idxs = NULL, is->count = 0, is->capacity = 0;
  }
}

static void indices_insert(Indices *is, long new) {
  assert(is->capacity < 100);
  if (is->count + 1 > is->capacity) {
    is->capacity += 5;
    is->idxs = reallocarray(is->idxs, is->capacity, sizeof(size_t));
  }
  is->idxs[is->count] = new;
  is->count++;
}

static Parser obj_parse(FILE *f) {
  char *buf = calloc(buffsize, sizeof(char));
  Parser parser = parser_new();
  while (fgets(buf, buffsize, f)) {
    buf[strcspn(buf, "\n")] = '\0';
    switch (buf[0]) {
    case 'v': {
      float x, y, z;
      int ret;
      if ((ret = sscanf(buf, "v %f %f %f", &x, &y, &z)), ret == 3) {
        parser_insert_vertex(&parser, point(x, y, z));
      } else {
        fprintf(stderr, "ERROR: malformed vertex line `%s`", buf);
        exit(EXIT_FAILURE);
      }
      break;
    }
    case 'f': {
      Indices indices = indices_new();
      char *saved;
      char *token = strtok_r(buf, " ", &saved); // 'f'
      while ((token = strtok_r(NULL, " ", &saved))) {
        char *endptr;
        errno = 0;
        const long a = strtol(token, &endptr, 10);
        if (errno != 0) {
          perror("strtol");
          exit(EXIT_FAILURE);
        }
        if (endptr == token) {
          fprintf(stderr, "no digits found\n");
          exit(EXIT_FAILURE);
        }
        indices_insert(&indices, a);
      }
      assert(indices.count >= 3);
      for (size_t i = 1; i < indices.count - 1; i++) {
        const Point a = parser.vertices[indices.idxs[0 + 0] - 1];
        const Point b = parser.vertices[indices.idxs[i + 0] - 1];
        const Point c = parser.vertices[indices.idxs[i + 1] - 1];
        Shape t = triangle(a, b, c);
        group_add(&parser.default_group, &t);
      }
      indices_free(&indices);
      break;
    }
    default:
      parser.n_ignored_lines++;
      break;
    }
  }
  free(buf);
  return parser;
}

Parser obj_parse_file(const char *filename) {
  FILE *f = fopen(filename, "r");
  if (!f) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }
  Parser parser = obj_parse(f);
  fclose(f);
  return parser;
}

void obj_parser_free(Parser *parser) {
  if (parser->vertices) {
    free(parser->vertices);
    parser->vertices = NULL, parser->capacity = 0, parser->count = 0;
  }
  group_free(&parser->default_group);
}
