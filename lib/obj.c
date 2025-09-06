#include "./obj.h"
#include <stdio.h>

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

static Parser obj_parse(FILE *f) {
  char *buf = calloc(buffsize, sizeof(char));
  Parser parser = parser_new();
  while (fgets(buf, buffsize, f)) {
    char rtype;
    float x, y, z;
    int ret;
    if ((ret = sscanf(buf, "%c %f %f %f", &rtype, &x, &y, &z)), ret == 4) {
      switch (rtype) {
      case 'v': {
        parser_insert_vertex(&parser, point(x, y, z));
        break;
      }
      case 'f': {
        const Point a = parser.vertices[(int)x - 1];
        const Point b = parser.vertices[(int)y - 1];
        const Point c = parser.vertices[(int)z - 1];
        Shape t = triangle(a, b, c);
        group_add(&parser.default_group, &t);
        break;
      }
      default:
        puts(buf);
        parser.n_ignored_lines++;
        break;
      }
    } else if (ret == EOF) {
      puts("EOF");
      break;
    } else {
      parser.n_ignored_lines++;
    }
  }
  printf("Wonky lines: %li\n", parser.n_ignored_lines);
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
