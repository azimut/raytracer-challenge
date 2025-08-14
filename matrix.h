#ifndef MATRIX_H
#define MATRIX_H

#include "./tuple.h"
#include <stdlib.h>

typedef struct Mat4 {
  float m[4][4];
} Mat4;

Mat4 m4_identity();
float m4_get(Mat4, size_t, size_t);
Mat4 m4_set(Mat4, size_t, size_t, float);
bool m4_equal(Mat4, Mat4);
Mat4 m4_mul(Mat4, Mat4);
Tuple m4_tmul(Mat4, Tuple);
Mat4 m4_transpose(Mat4);
Mat4 m4_inverse(Mat4);
void m4_print(Mat4);

#endif /* MATRIX_H */
