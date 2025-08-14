#include "./matrix.h"
#include "./util.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

typedef struct Mat3 {
  float m[3][3];
} Mat3;
typedef struct Mat2 {
  float m[2][2];
} Mat2;

Mat4 m4_identity() {
  return (Mat4){{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}};
}
static Mat3 m3_identity() { return (Mat3){{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}}; }
/* static Mat2 m2_identity() { return (Mat2){{{1, 0}, {0, 1}}}; } */

float m4_get(Mat4 m4, size_t row, size_t col) { return m4.m[row][col]; }

Mat4 m4_set(Mat4 m4, size_t row, size_t col, float val) {
  m4.m[row][col] = val;
  return m4;
}

bool near(float x, float y) { return fabs(x - y) < EPSILON; }
bool m3_equal(Mat3 a, Mat3 b) {
  return near(a.m[0][0], b.m[0][0]) && near(a.m[0][1], b.m[0][1]) &&
         near(a.m[0][2], b.m[0][2]) && near(a.m[1][0], b.m[1][0]) &&
         near(a.m[1][1], b.m[1][1]) && near(a.m[1][2], b.m[1][2]) &&
         near(a.m[2][0], b.m[2][0]) && near(a.m[2][1], b.m[2][1]) &&
         near(a.m[2][2], b.m[2][2]) && near(a.m[3][0], b.m[3][0]) &&
         near(a.m[3][1], b.m[3][1]) && near(a.m[3][2], b.m[3][2]);
}
bool m4_equal(Mat4 a, Mat4 b) {
  return near(a.m[0][0], b.m[0][0]) && near(a.m[0][1], b.m[0][1]) &&
         near(a.m[0][2], b.m[0][2]) && near(a.m[0][3], b.m[0][3]) &&
         near(a.m[1][0], b.m[1][0]) && near(a.m[1][1], b.m[1][1]) &&
         near(a.m[1][2], b.m[1][2]) && near(a.m[1][3], b.m[1][3]) &&
         near(a.m[2][0], b.m[2][0]) && near(a.m[2][1], b.m[2][1]) &&
         near(a.m[2][2], b.m[2][2]) && near(a.m[2][3], b.m[2][3]) &&
         near(a.m[3][0], b.m[3][0]) && near(a.m[3][1], b.m[3][1]) &&
         near(a.m[3][2], b.m[3][2]) && near(a.m[3][3], b.m[3][3]);
}

Mat4 m4_mul(Mat4 a, Mat4 b) {
  Mat4 res;
  for (size_t row = 0; row < 4; ++row) {
    for (size_t col = 0; col < 4; ++col) {
      res.m[row][col] = a.m[row][0] * b.m[0][col] + a.m[row][1] * b.m[1][col] +
                        a.m[row][2] * b.m[2][col] + a.m[row][3] * b.m[3][col];
    }
  }
  return res;
}

Tuple m4_tmul(Mat4 m, Tuple t) {
  return (Tuple){
      m.m[0][0] * t.x + m.m[0][1] * t.y + m.m[0][2] * t.z + m.m[0][3] * t.w,
      m.m[1][0] * t.x + m.m[1][1] * t.y + m.m[1][2] * t.z + m.m[1][3] * t.w,
      m.m[2][0] * t.x + m.m[2][1] * t.y + m.m[2][2] * t.z + m.m[2][3] * t.w,
      m.m[3][0] * t.x + m.m[3][1] * t.y + m.m[3][2] * t.z + m.m[3][3] * t.w,
  };
}

Mat4 m4_transpose(Mat4 m) {
  return (Mat4){{{m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0]},
                 {m.m[0][1], m.m[1][1], m.m[2][1], m.m[3][1]},
                 {m.m[0][2], m.m[1][2], m.m[2][2], m.m[3][2]},
                 {m.m[0][3], m.m[1][3], m.m[2][3], m.m[3][3]}}};
}

// Inversion - START

/* static float m2_determinant(Mat2 m2) { */
/*   return (m2.m[0][0] * m2.m[1][1]) - (m2.m[0][1] * m2.m[1][0]); */
/* } */

static Mat2 m3_submatrix(Mat3 m3, size_t skip_row, size_t skip_col) {
  Mat2 result;
  size_t trow = 0, tcol = 0;
  for (size_t row = 0; row < 3; row++) {
    if (row == skip_row)
      continue;
    for (size_t col = 0; col < 3; col++) {
      if (col == skip_col)
        continue;
      result.m[trow][tcol] = m3.m[row][col];
      tcol++;
    }
    trow++;
  }
  return result;
}

static Mat3 m4_submatrix(Mat4 m4, size_t skip_row, size_t skip_col) {
  Mat3 result;
  result.m[0][0] = 0;
  size_t trow = 0;
  for (size_t row = 0; row < 4; row++) {
    size_t tcol = 0;
    if (row == skip_row)
      continue;
    for (size_t col = 0; col < 4; col++) {
      if (col == skip_col)
        continue;
      result.m[trow][tcol] = m4.m[row][col];
      tcol++;
    }
    trow++;
  }
  return result;
}

/* static float m3_minor(Mat3 m3, size_t row, size_t col) { */
/*   return m2_determinant(m3_submatrix(m3, row, col)); */
/* } */

/* static float m3_cofactor(Mat3 m3, size_t row, size_t col) { */
/*   float minor = m3_minor(m3, row, col); */
/*   return ((row + col) % 2 == 0) ? minor : -minor; */
/* } */

/* static float m3_determinant(Mat3 m3) { */
/*   float result = 0; */
/*   for (size_t col; col < 3; col++) { */
/*     result = result + m3.m[0][col] * m3_cofactor(m3, 0, col); */
/*   } */
/*   return result; */
/* } */
/* static float m4_determinant(Mat4 m4) { return 0; } */
/* Mat4 m4_invert(Mat4 m) { return (Mat4){0}; } */

// INVERSION - END

void m4_print(Mat4 m4) {
  for (size_t row = 0; row < 4; row++)
    printf("%.2f %.2f %.2f %.2f\n", m4.m[row][0], m4.m[row][1], m4.m[row][2],
           m4.m[row][3]);
}
void m3_print(Mat3 m3) {
  for (size_t row = 0; row < 3; row++)
    printf("%.2f %.2f %.2f\n", m3.m[row][0], m3.m[row][1], m3.m[row][2]);
}
void m2_print(Mat2 m2) {
  for (size_t row = 0; row < 2; row++)
    printf("%.2f %.2f\n", m2.m[row][0], m2.m[row][1]);
}

int main(void) {
  m4_print(m4_identity());
  assert(m4_equal(m4_identity(), m4_identity()));
  assert(m3_equal(m4_submatrix(m4_identity(), 0, 0), m3_identity()));
  assert(m3_equal(m4_submatrix(m4_identity(), 3, 3), m3_identity()));
  return 0;
}
