#include "./matrix.h"
#include "./tuple.h"
#include "./util.h"
#include <stdio.h>

Mat2 m2(double a, double b, double c, double d) {
  return (Mat2){{{a, b}, {c, d}}};
}
Mat3 m3(double a, double b, double c, double d, double e, double f, double g,
        double h, double i) {
  return (Mat3){{{a, b, c}, {d, e, f}, {g, h, i}}};
}

bool m2_equal(const Mat2 a, const Mat2 b) {
  return near(a.m[0][0], b.m[0][0]) && near(a.m[0][1], b.m[0][1]) &&
         near(a.m[1][0], b.m[1][0]) && near(a.m[1][1], b.m[1][1]);
}
bool m3_equal(const Mat3 a, const Mat3 b) {
  return near(a.m[0][0], b.m[0][0]) && near(a.m[0][1], b.m[0][1]) &&
         near(a.m[0][2], b.m[0][2]) && near(a.m[1][0], b.m[1][0]) &&
         near(a.m[1][1], b.m[1][1]) && near(a.m[1][2], b.m[1][2]) &&
         near(a.m[2][0], b.m[2][0]) && near(a.m[2][1], b.m[2][1]) &&
         near(a.m[2][2], b.m[2][2]);
}
bool m4_equal(const Mat4 a, const Mat4 b) {
  return near(a.m[0][0], b.m[0][0]) && near(a.m[0][1], b.m[0][1]) &&
         near(a.m[0][2], b.m[0][2]) && near(a.m[0][3], b.m[0][3]) &&
         near(a.m[1][0], b.m[1][0]) && near(a.m[1][1], b.m[1][1]) &&
         near(a.m[1][2], b.m[1][2]) && near(a.m[1][3], b.m[1][3]) &&
         near(a.m[2][0], b.m[2][0]) && near(a.m[2][1], b.m[2][1]) &&
         near(a.m[2][2], b.m[2][2]) && near(a.m[2][3], b.m[2][3]) &&
         near(a.m[3][0], b.m[3][0]) && near(a.m[3][1], b.m[3][1]) &&
         near(a.m[3][2], b.m[3][2]) && near(a.m[3][3], b.m[3][3]);
}

Mat4 m4_mul(const Mat4 a, const Mat4 b) {
  Mat4 res;
  for (size_t row = 0; row < 4; ++row) {
    for (size_t col = 0; col < 4; ++col) {
      res.m[row][col] = a.m[row][0] * b.m[0][col] + a.m[row][1] * b.m[1][col] +
                        a.m[row][2] * b.m[2][col] + a.m[row][3] * b.m[3][col];
    }
  }
  return res;
}

Tuple m4_tmul(const Mat4 m, const Tuple t) {
  return (Tuple){
      m.m[0][0] * t.x + m.m[0][1] * t.y + m.m[0][2] * t.z + m.m[0][3] * t.w,
      m.m[1][0] * t.x + m.m[1][1] * t.y + m.m[1][2] * t.z + m.m[1][3] * t.w,
      m.m[2][0] * t.x + m.m[2][1] * t.y + m.m[2][2] * t.z + m.m[2][3] * t.w,
      m.m[3][0] * t.x + m.m[3][1] * t.y + m.m[3][2] * t.z + m.m[3][3] * t.w,
  };
}

static Mat4 m4_sdiv(const Mat4 m, const double by) {
  Mat4 result;
  for (size_t row = 0; row < 4; row++) {
    for (size_t col = 0; col < 4; col++) {
      result.m[row][col] = m.m[row][col] / by;
    }
  }
  return result;
}

Mat4 m4_transpose(const Mat4 m) {
  return (Mat4){{{m.m[0][0], m.m[1][0], m.m[2][0], m.m[3][0]},
                 {m.m[0][1], m.m[1][1], m.m[2][1], m.m[3][1]},
                 {m.m[0][2], m.m[1][2], m.m[2][2], m.m[3][2]},
                 {m.m[0][3], m.m[1][3], m.m[2][3], m.m[3][3]}}};
}

// Inversion - START

static double m2_determinant(const Mat2 m2) {
  return (m2.m[0][0] * m2.m[1][1]) - (m2.m[0][1] * m2.m[1][0]);
}

Mat2 m3_submatrix(const Mat3 m3, const size_t skip_row, const size_t skip_col) {
  Mat2 result = M2_IDENTITY;
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

Mat3 m4_submatrix(const Mat4 m4, const size_t skip_row, const size_t skip_col) {
  Mat3 result = {0};
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

static inline double m3_minor(const Mat3 m, const size_t row,
                              const size_t col) {
  return m2_determinant(m3_submatrix(m, row, col));
}
static inline double m3_cofactor(const Mat3 m3, const size_t row,
                                 const size_t col) {
  const double minor = m3_minor(m3, row, col);
  return ((row + col) % 2) ? -minor : minor;
}
static double m3_determinant(const Mat3 m3) {
  double result = 0;
  for (size_t col = 0; col < 3; col++) {
    result += m3.m[0][col] * m3_cofactor(m3, 0, col);
  }
  return result;
}

static inline double m4_minor(const Mat4 m4, const size_t row,
                              const size_t col) {
  return m3_determinant(m4_submatrix(m4, row, col));
}
static inline double m4_cofactor(const Mat4 m4, const size_t row,
                                 const size_t col) {
  const double minor = m4_minor(m4, row, col);
  return ((row + col) % 2) ? -minor : minor;
}
static double m4_determinant(const Mat4 m4) {
  double result = 0;
  for (size_t col = 0; col < 4; col++) {
    result += m4.m[0][col] * m4_cofactor(m4, 0, col);
  }
  return result;
}

/* static bool m4_is_invertible(Mat4 m4) { return m4_determinant(m4) != 0; } */

Mat4 m4_inverse(const Mat4 m) {
  const double determinant = m4_determinant(m);
  Mat4 result;
  for (size_t row = 0; row < 4; row++) {
    for (size_t col = 0; col < 4; col++) {
      result.m[row][col] = m4_cofactor(m, row, col);
    }
  }
  return m4_sdiv(m4_transpose(result), determinant);
}

// INVERSION - END

void m4_print(const Mat4 m4) {
  for (size_t row = 0; row < 4; row++)
    printf("%.2f %.2f %.2f %.2f\n", m4.m[row][0], m4.m[row][1], m4.m[row][2],
           m4.m[row][3]);
}
void m3_print(const Mat3 m3) {
  for (size_t row = 0; row < 3; row++)
    printf("%.2f %.2f %.2f\n", m3.m[row][0], m3.m[row][1], m3.m[row][2]);
}
void m2_print(const Mat2 m2) {
  for (size_t row = 0; row < 2; row++)
    printf("%.2f %.2f\n", m2.m[row][0], m2.m[row][1]);
}
