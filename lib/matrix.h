#ifndef MATRIX_H
#define MATRIX_H

#include "./tuple.h"
#include <stdlib.h>

typedef struct Mat4 {
  double m[4][4];
} Mat4;
typedef struct Mat3 {
  double m[3][3];
} Mat3;
typedef struct Mat2 {
  double m[2][2];
} Mat2;

#define M4_IDENTITY                                                            \
  ((Mat4){{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}})
#define M3_IDENTITY ((Mat3){{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}})
#define M2_IDENTITY ((Mat2){{{1, 0}, {0, 1}}})

Mat4 m4(double, double, double, double, double, double, double, double, double,
        double, double, double, double, double, double, double);
bool m4_equal(const Mat4, Mat4);
Mat4 m4_mul(const Mat4, Mat4);
Tuple m4_tmul(const Mat4, Tuple);
Mat4 m4_transpose(const Mat4);
Mat4 m4_inverse(const Mat4);
void m4_print(const Mat4);

Mat3 m3(double, double, double, double, double, double, double, double, double);
void m3_print(const Mat3);
bool m3_equal(const Mat3, Mat3);
Mat3 m4_submatrix(const Mat4, size_t, size_t);

Mat2 m2(double, double, double, double);
void m2_print(const Mat2);
bool m2_equal(const Mat2, Mat2);
Mat2 m3_submatrix(const Mat3, size_t, size_t);

#endif /* MATRIX_H */
