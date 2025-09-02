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

bool is_point(const Tuple);
bool is_vector(const Tuple);

bool tuple_equal(const Tuple, const Tuple);
Tuple tuple_add(const Tuple, const Tuple);
Tuple tuple_sub(const Tuple, const Tuple);
Tuple tuple_neg(const Tuple);
Tuple tuple_smul(const Tuple, double);
Tuple tuple_sdiv(const Tuple, double);
double tuple_length(const Tuple);
Tuple tuple_normalize(const Tuple);
double tuple_dot_product(const Tuple, const Tuple);
Tuple tuple_cross_product(const Tuple, const Tuple);

#endif /* TUPLE_H */
