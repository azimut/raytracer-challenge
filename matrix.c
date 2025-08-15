#include "./matrix.h"
#include "./tuple.h"
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

Mat2 m2(float a, float b, float c, float d) { return (Mat2){{{a, b}, {c, d}}}; }
Mat3 m3(float a, float b, float c, float d, float e, float f, float g, float h,
        float i) {
  return (Mat3){{{a, b, c}, {d, e, f}, {g, h, i}}};
}
Mat4 m4(float a, float b, float c, float d, float e, float f, float g, float h,
        float i, float j, float k, float l, float m, float n, float o,
        float p) {
  return (Mat4){{{a, b, c, d}, {e, f, g, h}, {i, j, k, l}, {m, n, o, p}}};
}

Mat4 m4_identity() {
  return (Mat4){{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}};
}
static Mat3 m3_identity() { return (Mat3){{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}}}; }
static Mat2 m2_identity() { return (Mat2){{{1, 0}, {0, 1}}}; }

float m4_get(Mat4 m4, size_t row, size_t col) { return m4.m[row][col]; }

Mat4 m4_set(Mat4 m4, size_t row, size_t col, float val) {
  m4.m[row][col] = val;
  return m4;
}

static bool m2_equal(Mat2 a, Mat2 b) {
  return near(a.m[0][0], b.m[0][0]) && near(a.m[0][1], b.m[0][1]) &&
         near(a.m[1][0], b.m[1][0]) && near(a.m[1][1], b.m[1][1]);
}
static bool m3_equal(Mat3 a, Mat3 b) {
  return near(a.m[0][0], b.m[0][0]) && near(a.m[0][1], b.m[0][1]) &&
         near(a.m[0][2], b.m[0][2]) && near(a.m[1][0], b.m[1][0]) &&
         near(a.m[1][1], b.m[1][1]) && near(a.m[1][2], b.m[1][2]) &&
         near(a.m[2][0], b.m[2][0]) && near(a.m[2][1], b.m[2][1]) &&
         near(a.m[2][2], b.m[2][2]);
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

static Mat4 m4_sdiv(Mat4 m, float by) {
  Mat4 result;
  for (size_t row = 0; row < 4; row++) {
    for (size_t col = 0; col < 4; col++) {
      result.m[row][col] = m.m[row][col] / by;
    }
  }
  return result;
}

Mat4 m4_transpose(Mat4 m) {
  return (Mat4){{{m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0]},
                 {m.m[0][1], m.m[1][1], m.m[2][1], m.m[3][1]},
                 {m.m[0][2], m.m[1][2], m.m[2][2], m.m[3][2]},
                 {m.m[0][3], m.m[1][3], m.m[2][3], m.m[3][3]}}};
}

// Inversion - START

static float m2_determinant(Mat2 m2) {
  return (m2.m[0][0] * m2.m[1][1]) - (m2.m[0][1] * m2.m[1][0]);
}

static Mat2 m3_submatrix(Mat3 m3, size_t skip_row, size_t skip_col) {
  Mat2 result = m2_identity();
  size_t trow = 0;
  for (size_t row = 0; row < 3; row++) {
    size_t tcol = 0;
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

static float m3_minor(Mat3 m, size_t row, size_t col) {
  return m2_determinant(m3_submatrix(m, row, col));
}
static float m3_cofactor(Mat3 m3, size_t row, size_t col) {
  float minor = m3_minor(m3, row, col);
  return ((row + col) % 2 == 0) ? minor : -minor;
}
static float m3_determinant(Mat3 m3) {
  float result = 0;
  for (size_t col = 0; col < 3; col++) {
    result += m3.m[0][col] * m3_cofactor(m3, 0, col);
  }
  return result;
}

static float m4_minor(Mat4 m4, size_t row, size_t col) {
  return m3_determinant(m4_submatrix(m4, row, col));
}
static float m4_cofactor(Mat4 m4, size_t row, size_t col) {
  float minor = m4_minor(m4, row, col);
  return ((row + col) % 2 == 0) ? minor : -minor;
}
static float m4_determinant(Mat4 m4) {
  float result = 0;
  for (size_t col = 0; col < 4; col++) {
    result += m4.m[0][col] * m4_cofactor(m4, 0, col);
  }
  return result;
}

static bool m4_is_invertible(Mat4 m4) { return m4_determinant(m4) != 0; }

Mat4 m4_inverse(Mat4 m) {
  float determinant = m4_determinant(m);
  Mat4 result;
  for (size_t row = 0; row < 4; row++) {
    for (size_t col = 0; col < 4; col++) {
      result.m[row][col] = m4_cofactor(m, row, col);
    }
  }
  return m4_sdiv(m4_transpose(result), determinant);
}

// INVERSION - END

void m4_print(Mat4 m4) {
  for (size_t row = 0; row < 4; row++)
    printf("%.2f %.2f %.2f %.2f\n", m4.m[row][0], m4.m[row][1], m4.m[row][2],
           m4.m[row][3]);
}
static void m3_print(Mat3 m3) {
  for (size_t row = 0; row < 3; row++)
    printf("%.2f %.2f %.2f\n", m3.m[row][0], m3.m[row][1], m3.m[row][2]);
}
static void m2_print(Mat2 m2) {
  for (size_t row = 0; row < 2; row++)
    printf("%.2f %.2f\n", m2.m[row][0], m2.m[row][1]);
}

int main2(void) {
  assert(m4_equal(m4_identity(), m4_identity()));
  assert(m3_equal(m4_submatrix(m4_identity(), 0, 0), m3_identity()));
  assert(m3_equal(m4_submatrix(m4_identity(), 3, 3), m3_identity()));
  assert(m2_equal(m2_identity(), m3_submatrix(m3_identity(), 0, 0)));
  Mat4 am4 = m4(1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2);
  assert(m4_equal(
      m4_mul(am4, m4(-2, 1, 2, 3, 3, 2, 1, -1, 4, 3, 6, 5, 1, 2, 7, 8)),
      m4(20, 22, 50, 48, 44, 54, 114, 108, 40, 58, 110, 102, 16, 26, 46, 42)));
  assert(m4_equal(m4_mul(am4, m4_identity()), am4));
  assert(tuple_equal(m4_tmul(m4(1, 2, 3, 4, 2, 4, 4, 2, 8, 6, 4, 1, 0, 0, 0, 1),
                             tuple(1, 2, 3, 1)),
                     tuple(18, 24, 33, 1)));
  assert(
      m4_equal(m4_transpose(m4(0, 9, 3, 0, 9, 8, 0, 8, 1, 8, 5, 3, 0, 0, 5, 8)),
               m4(0, 9, 1, 0, 9, 8, 8, 0, 3, 0, 5, 5, 0, 8, 3, 8)));
  assert(17 == m2_determinant(m2(1, 5, -3, 2)));
  assert(m2_equal(m3_submatrix(m3(1, 5, 0, -3, 2, 7, 0, 6, -3), 0, 2),
                  m2(-3, 2, 0, 6)));
  assert(m3_equal(
      m4_submatrix(m4(-6, 1, 1, 6, -8, 5, 8, 6, -1, 0, 8, 2, -7, 1, -1, 1), 2,
                   1),
      m3(-6, 1, 6, -8, 8, 6, -7, -1, 1)));
  Mat3 am3 = m3(3, 5, 0, 2, -1, -7, 6, -1, 5);
  assert(25 == m2_determinant(m3_submatrix(am3, 1, 0)));
  assert(25 == m3_minor(am3, 1, 0));
  assert(-12 == m3_minor(am3, 0, 0));
  assert(-12 == m3_cofactor(am3, 0, 0));
  assert(25 == m3_minor(am3, 1, 0));
  assert(-25 == m3_cofactor(am3, 1, 0));
  assert(56 == m3_cofactor(m3(1, 2, 6, -5, 8, -4, 2, 6, 4), 0, 0));
  assert(12 == m3_cofactor(m3(1, 2, 6, -5, 8, -4, 2, 6, 4), 0, 1));
  assert(-46 == m3_cofactor(m3(1, 2, 6, -5, 8, -4, 2, 6, 4), 0, 2));
  assert(-196 == m3_determinant(m3(1, 2, 6, -5, 8, -4, 2, 6, 4)));

  Mat4 cm4 = m4(-2, -8, 3, 5, -3, 1, 7, 3, 1, 2, -9, 6, -6, 7, 7, -9);
  assert(690 == m4_cofactor(cm4, 0, 0));
  assert(447 == m4_cofactor(cm4, 0, 1));
  assert(210 == m4_cofactor(cm4, 0, 2));
  assert(51 == m4_cofactor(cm4, 0, 3));
  assert(-4071 == m4_determinant(cm4));

  Mat4 m4i = m4(6, 4, 4, 4, 5, 5, 7, 6, 4, -9, 3, -7, 9, 1, 7, -6);
  assert(-2120 == m4_determinant(m4i));
  assert(m4_is_invertible(m4i));

  Mat4 m4ni = m4(-4, 2, -2, -3, 9, 6, 2, 6, 0, -5, 1, -5, 0, 0, 0, 0);
  assert(0 == m4_determinant(m4ni));
  assert(!m4_is_invertible(m4ni));

  Mat4 a4 = m4(-5, 2, 6, -8, 1, -5, 1, 8, 7, 7, -6, -7, 1, -3, 7, 4);
  Mat4 a4i = m4(0.21805, 0.45113, 0.24060, -0.04511, -0.80827, -1.45677,
                -0.44361, 0.52068, -0.07895, -0.22368, -0.05263, 0.19737,
                -0.52256, -0.81391, -0.30075, 0.30639);
  assert(532 == m4_determinant(a4));
  assert(-160 == m4_cofactor(a4, 2, 3));
  assert(105 == m4_cofactor(a4, 3, 2));
  assert(m4_equal(m4_inverse(a4), a4i));
  printf("ALL OK?\n");
  return 0;
}
