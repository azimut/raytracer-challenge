#ifndef TUPLE_H
#define TUPLE_H

#include <stdbool.h>

typedef struct Tuple {
  double x, y, z, w;
} Tuple;
typedef Tuple Point;
typedef Tuple Vector;

Tuple tuple(double, double, double, double);
Tuple point(double, double, double);
Tuple vector(double, double, double);

bool is_point(Tuple);
bool is_vector(Tuple);

bool tuple_equal(Tuple, Tuple);
Tuple tuple_add(Tuple, Tuple);
Tuple tuple_sub(Tuple, Tuple);
Tuple tuple_neg(Tuple);
Tuple tuple_smul(Tuple, double);
Tuple tuple_sdiv(Tuple, double);
double tuple_length(Tuple);
Tuple tuple_normalize(Tuple);
double tuple_dot_product(Tuple, Tuple);
Tuple tuple_cross_product(Tuple, Tuple);

#endif /* TUPLE_H */
