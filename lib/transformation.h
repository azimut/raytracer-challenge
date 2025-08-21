#ifndef TRANSLATION_H
#define TRANSLATION_H

#include "./matrix.h"

Mat4 translation(float, float, float);
Mat4 scaling(float, float, float);
Mat4 rotation_x(float);
Mat4 rotation_y(float);
Mat4 rotation_z(float);
Mat4 shearing(float, float, float, float, float, float);
float radians(float);

#endif /* TRANSLATION_H */
