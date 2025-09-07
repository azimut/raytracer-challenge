#ifndef UTIL_H
#define UTIL_H

#include <stdbool.h>

#define EPSILON 0.0001
#define ARRAY_LENGTH(array) (sizeof(array) / sizeof((array)[0]))

double clamp(double, double, double);
bool near(double, double);

#endif /* UTIL_H */
