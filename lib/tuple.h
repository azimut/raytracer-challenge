#ifndef TUPLE_H
#define TUPLE_H

#include <stdbool.h>

typedef struct Tuple {
  double x, y, z, w;
} Tuple;
typedef Tuple Point;
typedef Tuple Vector;

Tuple tuple(double, double, double, double);

#define POINT(x, y, z) ((Point){x, y, z, 1})
#define VECTOR(x, y, z) ((Vector){x, y, z, 0})

bool is_point(const Tuple);
bool is_vector(const Tuple);

bool tuple_equal(const Tuple, const Tuple);
Tuple tuple_add(const Tuple, const Tuple);
Tuple tuple_sub(const Tuple, const Tuple);
Tuple tuple_neg(const Tuple);
Tuple tuple_smul(const Tuple, const double);
Tuple tuple_sdiv(const Tuple, const double);
double tuple_length(const Tuple);
Tuple tuple_normalize(const Tuple);
double tuple_dot_product(const Tuple, const Tuple);
Tuple tuple_cross_product(const Tuple, const Tuple);

#endif /* TUPLE_H */
