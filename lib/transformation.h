#ifndef TRANSLATION_H
#define TRANSLATION_H

#include "./matrix.h"

Mat4 translation(double, double, double);
Mat4 scaling(double, double, double);
Mat4 rotation(double, double, double);
Mat4 rotation_x(double);
Mat4 rotation_y(double);
Mat4 rotation_z(double);
Mat4 shearing(double, double, double, double, double, double);
double radians(double);

#endif /* TRANSLATION_H */
