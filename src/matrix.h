#ifndef MATRIX_H
#define MATRIX_H

#include "./tuple.h"
#include <stdlib.h>

typedef struct Mat4 {
  float m[4][4];
} Mat4;

Mat4 m4(float, float, float, float, float, float, float, float, float, float,
        float, float, float, float, float, float);
Mat4 m4_identity();
float m4_get(Mat4, size_t, size_t);
Mat4 m4_set(Mat4, size_t, size_t, float);
bool m4_equal(Mat4, Mat4);
Mat4 m4_mul(Mat4, Mat4);
Tuple m4_tmul(Mat4, Tuple);
Mat4 m4_transpose(Mat4);
Mat4 m4_inverse(Mat4);
void m4_print(Mat4);

typedef struct Mat3 {
  float m[3][3];
} Mat3;
Mat3 m3(float, float, float, float, float, float, float, float, float);
void m3_print(Mat3);
Mat3 m3_identity();
bool m3_equal(Mat3, Mat3);
Mat3 m4_submatrix(Mat4, size_t, size_t);

typedef struct Mat2 {
  float m[2][2];
} Mat2;

Mat2 m2(float, float, float, float);
void m2_print(Mat2);
Mat2 m2_identity();
bool m2_equal(Mat2, Mat2);
Mat2 m3_submatrix(Mat3, size_t, size_t);

#endif /* MATRIX_H */
