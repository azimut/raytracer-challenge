#include "./transformation.h"
#include "./tuple.h"
#include <math.h>

double radians(double degrees) { return (degrees / 180.0) * M_PI; }

Mat4 translation(double x, double y, double z) {
  return m4(1, 0, 0, x, 0, 1, 0, y, 0, 0, 1, z, 0, 0, 0, 1);
}

Mat4 rotation_x(double rad) {
  return m4(1, 0, 0, 0, 0, cos(rad), -sin(rad), 0, 0, sin(rad), cos(rad), 0, 0,
            0, 0, 1);
}

Mat4 rotation_y(double rad) {
  return m4(cos(rad), 0, sin(rad), 0, 0, 1, 0, 0, -sin(rad), 0, cos(rad), 0, 0,
            0, 0, 1);
}

Mat4 rotation_z(double rad) {
  return m4(cos(rad), -sin(rad), 0, 0, sin(rad), cos(rad), 0, 0, 0, 0, 1, 0, 0,
            0, 0, 1);
}

Mat4 scaling(double x, double y, double z) {
  return m4(x, 0, 0, 0, 0, y, 0, 0, 0, 0, z, 0, 0, 0, 0, 1);
}

Mat4 shearing(double xy, double xz, double yx, double yz, double zx,
              double zy) {
  return m4(1, xy, xz, 0, yx, 1, yz, 0, zx, zy, 1, 0, 0, 0, 0, 1);
}
