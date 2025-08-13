#include "./matrix.h"
#include "./util.h"
#include <stdio.h>

typedef struct Mat3 {
  float m[3][3];
} Mat3;
typedef struct Mat2 {
  float m[2][2];
} Mat2;

Mat4 m4_identity() {
  return (Mat4){1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
}
static Mat3 m3_identity() { return (Mat3){1, 0, 0, 0, 1, 0, 0, 0, 1}; }
static Mat2 m2_identity() { return (Mat2){1, 0, 0, 1}; }

float m4_get(Mat4 m4, size_t row, size_t col) { return m4.m[row][col]; }

Mat4 m4_set(Mat4 m4, size_t row, size_t col, float val) {
  m4.m[row][col] = val;
  return m4;
}

bool m4_equal(Mat4 m, Mat4 n) {
  return near(m.m[0][0], n.m[0][0]) && near(m.m[0][1], n.m[0][1]) &&
         near(m.m[0][2], n.m[0][2]) && near(m.m[0][3], n.m[0][3]) &&
         near(m.m[1][0], n.m[1][0]) && near(m.m[1][1], n.m[1][1]) &&
         near(m.m[1][2], n.m[1][2]) && near(m.m[1][3], n.m[1][3]) &&
         near(m.m[2][0], n.m[2][0]) && near(m.m[2][1], n.m[2][1]) &&
         near(m.m[2][2], n.m[2][2]) && near(m.m[2][3], n.m[2][3]) &&
         near(m.m[3][0], n.m[3][0]) && near(m.m[3][1], n.m[3][1]) &&
         near(m.m[3][2], n.m[3][2]) && near(m.m[3][3], n.m[3][3]);
}

void m4_print(Mat4 m4) {
  for (size_t row = 0; row < 4; row++)
    printf("%.2f %.2f %.2f %.2f\n", m4.m[row][0], m4.m[row][1], m4.m[row][2],
           m4.m[row][3]);
}

int main(void) {
  Mat4 id = m4_identity();
  m4_print(id);
  return 0;
}
