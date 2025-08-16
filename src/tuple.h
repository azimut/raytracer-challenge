#ifndef TUPLE_H
#define TUPLE_H

#include <stdbool.h>

typedef struct Tuple {
  float x, y, z, w;
} Tuple;
typedef Tuple Point;
typedef Tuple Vector;

Tuple tuple(float, float, float, float);
Tuple point(float, float, float);
Tuple vector(float, float, float);

bool tuple_equal(Tuple, Tuple);
Tuple tuple_add(Tuple, Tuple);
Tuple tuple_sub(Tuple, Tuple);
Tuple tuple_neg(Tuple);
Tuple tuple_smul(Tuple, float);
Tuple tuple_sdiv(Tuple, float);
float tuple_length(Tuple);
Tuple tuple_normalize(Tuple);
float tuple_dot_product(Tuple, Tuple);
Tuple tuple_cross_product(Tuple, Tuple);

#endif /* TUPLE_H */
